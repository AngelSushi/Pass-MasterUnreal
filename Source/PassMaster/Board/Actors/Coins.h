#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coins.generated.h"

UCLASS()
class PASSMASTER_API ACoins : public AActor
{
	GENERATED_BODY()
	
public:	
	ACoins();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;

protected:
	virtual void BeginPlay() override;


};
