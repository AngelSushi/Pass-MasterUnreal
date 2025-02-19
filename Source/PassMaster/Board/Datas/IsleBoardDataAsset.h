#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../../GameManager.h"
#include "IsleBoardDataAsset.generated.h"

class ADice;
class APassMasterCharacter;
class UMaterialInstanceDynamic;

UCLASS()
class PASSMASTER_API UIsleBoardDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADice> Dice;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACoins> Coins;

	UPROPERTY(EditAnywhere)
	int16 PlayerCount;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EGameState> BeginGameState;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APassMasterCharacter>> PlayersActor;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class AActor> Grid;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UMaterial> PathMaterial;
};
