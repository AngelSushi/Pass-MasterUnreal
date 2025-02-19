#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../../GameManager.h"
#include "BoardSubSystem.generated.h"

class UIsleBoardDataAsset;
class UGameManager;
class APassMasterGameMode;
class APassMasterCharacter;

// Turn Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginTurnEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBeginPlayerTurnEvent,APassMasterCharacter*,Character,UBoardSubSystem*, BoardSystem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEndPlayerTurnEvent,APassMasterCharacter*,Character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndTurnEvent);

// Order Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginOrderEvent, UGameManager*,GM);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateOrderEvent, APassMasterCharacter*, Character, int16, OrderResult);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndOrderEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartTransitionEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndTransitionEvent);

// Step Event 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPassOverStepEvent, APassMasterCharacter*,Character, AStep*, Step);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArriveOnStepEvent, APassMasterCharacter*, Character, AStep*, Step);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLeaveStepEvent, APassMasterCharacter*, Character, AStep*, Step);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStartCoinsAnimEvent, APassMasterCharacter*, Character, int16, Coins);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndCoinsAnimEvent);


UCLASS()
class PASSMASTER_API UBoardSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	

public:

	// Turn Events
	UPROPERTY(VisibleAnywhere)
	FOnBeginTurnEvent OnBeginTurnEvent;

	UPROPERTY(VisibleAnywhere,BlueprintAssignable,BlueprintCallable)
	FOnBeginPlayerTurnEvent OnBeginPlayerTurnEvent;

	UPROPERTY(VisibleAnywhere)
	FOnEndPlayerTurnEvent OnEndPlayerTurnEvent;

	UPROPERTY(VisibleAnywhere)
	FOnEndTurnEvent OnEndTurnEvent;

	// Order Events
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

	// Step Events
	UPROPERTY(VisibleAnywhere)
	FOnPassOverStepEvent OnPassOverStepEvent;

	UPROPERTY(VisibleAnywhere)
	FOnArriveOnStepEvent OnArriveOnStepEvent;

	UPROPERTY(VisibleAnywhere)
	FOnLeaveStepEvent OnLeaveStepEvent;

	UPROPERTY(VisibleAnywhere)
	FOnStartCoinsAnimEvent OnStartCoinsAnimEvent;

	UPROPERTY(VisibleAnywhere)
	FOnEndCoinsAnimEvent OnEndCoinsAnimEvent;
	
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	void Deinitialize() override;


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
	void OnEndCoinsAnim();

	UFUNCTION()
	ADice* InitDice(APassMasterCharacter* Character);

private:
	UPROPERTY()
	UIsleBoardDataAsset* BoardAsset;

	UPROPERTY()
	ACameraActor* MainCamera;

	float ActualPlayerIndex = -1; UPROPERTY()
	
	APassMasterCharacter* ManagerOfCamera;

	
public:

	UFUNCTION()
	void OnBeginTurn();

	UFUNCTION()
	void OnEndTurn();

	UFUNCTION()
	void OnPlayerBeginTurn();

	UFUNCTION(BlueprintCallable)
	void OnPlayerEndTurn();

	UFUNCTION(BlueprintCallable)
	APassMasterCharacter* GetActualPlayer();

	UFUNCTION(BlueprintCallable)
	ACameraActor* GetMainCamera() { return MainCamera; }

	UFUNCTION()
	APassMasterCharacter* GetManagerOfCamera() { return ManagerOfCamera; }

	UFUNCTION()
	UIsleBoardDataAsset* GetBoardAsset() { return BoardAsset; }

public:

	UPROPERTY()
	TObjectPtr<class UGameManager> GameManager;
};
