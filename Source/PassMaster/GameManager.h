#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameManager.generated.h"

//#define ECC_STEP ECC_GameTraceChannel1;

class AStep;

UENUM()
enum EGameState : uint8 {
	NONE,
	CHOOSE_ORDER,
	PARTYGAME,
	CHOOSE_MINIGAME,
	MINIGAME,
	FREE,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSetupBoardFinishedEvent, UGameManager*,GameManager, APassMasterGameMode*, GameMode);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDatasPlayerChangedEvent, TArray<FPlayerData>&, PlayersData);

UCLASS()
class PASSMASTER_API UGameManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnywhere)
	FOnSetupBoardFinishedEvent OnSetupBoardFinished;

	//UPROPERTY(VisibleAnywhere)
	//FOnDatasPlayerChangedEvent OnDatasPlayerChanged;

//	UPROPERTY(EditAnywhere,BlueprintReadWrite)
//	TArray<FPlayerData> PlayersData;

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

	UPROPERTY()
	TObjectPtr<class AStep> FirstStep;

	UFUNCTION(BlueprintCallable)
	void UpdateDatas();

private:
	UFUNCTION()
	void OrderPlayersStart();

	UFUNCTION()
	void AssignSteps();

	UFUNCTION()
	AStep* FindFirstStep();

	UFUNCTION()
	void BoxRaycast(AActor* Actor,FVector Direction,TArray<FHitResult>& HitResults,bool bDebug,FVector Size = FVector(200.F,30.F,30.F));

	UFUNCTION()
	AActor* FindClosestStep(AActor* Actor,TArray<FHitResult>& HitResults,FVector Direction);
};
