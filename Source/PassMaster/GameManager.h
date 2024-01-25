#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameManager.generated.h"

//#define ECC_STEP ECC_GameTraceChannel1;

struct FPlayerData;
class AStep;

UENUM()
enum EGameState : uint8 {
	CHOOSE_ORDER,
	PARTYGAME,
	CHOOSE_MINIGAME,
	MINIGAME,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSetupBoardFinished, UGameManager*,GameManager, APassMasterGameMode*, GameMode);

UCLASS()
class PASSMASTER_API UGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere)
	FOnSetupBoardFinished OnSetupBoardFinished;

	UPROPERTY(EditAnywhere)
	TArray<FPlayerData> PlayersData;

	UPROPERTY(EditAnywhere)
	int16 PlayerCount;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EGameState> GameState;

public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	UFUNCTION()
	void SetupBoard();

	UPROPERTY()
	TArray<AActor*> PlayersStart;

private:
	UFUNCTION()
	void OrderPlayersStart();

	UFUNCTION()
	void AssignSteps();

	UFUNCTION()
	AStep* FindFirstStep();

	UFUNCTION()
	void BoxRaycast(AActor* Actor,FVector Direction,TArray<FHitResult>& HitResults,bool bDebug);

	UFUNCTION()
	AActor* FindClosestStep(AActor* Actor,TArray<FHitResult>& HitResults,FVector Direction);

};
