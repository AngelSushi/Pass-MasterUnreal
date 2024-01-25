#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dice.generated.h"

UCLASS()
class PASSMASTER_API ADice : public AActor
{
	GENERATED_BODY()
	
public:	
	ADice();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Scene;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> Mesh;


private:
	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	TArray<UMaterial*> DiceMaterials;

	UPROPERTY()
	TObjectPtr<class ABoardHUD> HUD;

	UPROPERTY()
	UGameManager* GameManager;

	UPROPERTY()
	UBoardSubSystem* BoardSystem;

	float Timer;

	float DiceIndex = 1;

protected:
	virtual void BeginPlay() override;

private:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
