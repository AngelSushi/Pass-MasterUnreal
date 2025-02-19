#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridSpawner.generated.h"


UCLASS()
class PASSMASTER_API AGridSpawner : public AActor {
	GENERATED_BODY()
	
public:	
	AGridSpawner();
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> SceneComponent;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> First;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBillboardComponent> FirstBillboard;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Second;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBillboardComponent> SecondBillboard;

public:	

};
