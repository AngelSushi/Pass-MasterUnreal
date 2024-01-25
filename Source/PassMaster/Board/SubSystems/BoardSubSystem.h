#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BoardSubSystem.generated.h"

class UIsleBoardDataAsset;
class UGameManager;
class APassMasterGameMode;
class APassMasterCharacter;
enum EGameState;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginTurnEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeginPlayerTurnEvent,APassMasterCharacter*,Character,UBoardSubSystem*, BoardSystem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndPlayerTurnEvent,APassMasterCharacter*,Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginOrderEvent, UGameManager*,GM);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateOrderEvent, APassMasterCharacter*, Character, int16, OrderResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndOrderEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartTransitionEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndTransitionEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndTurnEvent);	

UCLASS()
class PASSMASTER_API UBoardSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	

public:

	UPROPERTY(VisibleAnywhere)
	FOnBeginTurnEvent OnBeginTurnEvent;

	UPROPERTY(VisibleAnywhere,BlueprintAssignable,BlueprintCallable)
	FOnBeginPlayerTurnEvent OnBeginPlayerTurnEvent;

	UPROPERTY(VisibleAnywhere)
	FOnEndPlayerTurnEvent OnEndPlayerTurnEvent;

	UPROPERTY(VisibleAnywhere)
	FOnBeginOrderEvent OnBeginOrderEvent;

	UPROPERTY(VisibleAnywhere)
	FOnUpdateOrderEvent OnUpdateOrderEvent;

	UPROPERTY(VisibleAnywhere)
	FOnEndOrderEvent OnEndOrderEvent;

	UPROPERTY(VisibleAnywhere)
	FOnStartTransitionEvent OnStartTransitionEvent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,BlueprintAssignable,BlueprintCallable)
	FOnEndTransitionEvent OnEndTransitionEvent;

	UPROPERTY(VisibleAnywhere)
	FOnEndTurnEvent OnEndTurnEvent;

	
public:

	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	UFUNCTION()
	void OnSetupBoardFinished(UGameManager* GM,APassMasterGameMode* GameMode);

	UFUNCTION()
	void OnBeginOrder(UGameManager* GM);

	UFUNCTION()
	void OnUpdateOrder(APassMasterCharacter* Character,int16 DiceResult);

	UFUNCTION()
	void OnEndOrder();

	UFUNCTION()
	void OnStartTransition();

	UFUNCTION()
	void OnEndTransition();

	UFUNCTION()
	void InitDice(APassMasterCharacter* Character);

private:
	UPROPERTY()
	UIsleBoardDataAsset* BoardAsset;

	UPROPERTY()
	TObjectPtr<class UGameManager> GameManager;

	UPROPERTY()
	ACameraActor* MainCamera;

	float ActualPlayerIndex = -1;
	
public:

	UFUNCTION()
	void OnBeginTurn();

	UFUNCTION()
	void OnEndTurn();

	UFUNCTION()
	void OnPlayerBeginTurn();

	UFUNCTION()
	void OnPlayerEndTurn();

	UFUNCTION(BlueprintCallable)
	APassMasterCharacter* GetActualPlayer();

	UFUNCTION(BlueprintCallable)
	ACameraActor* GetMainCamera() { return MainCamera; }
};
