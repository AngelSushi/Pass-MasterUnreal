#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../Interfaces/Collisionnable.h"
#include "Step.generated.h"


class ICollisionnable;
class IOrderEventAssignable;

UCLASS()
class PASSMASTER_API AStep : public AActor,public ICollisionnable
{
	GENERATED_BODY()
	
public:	
	AStep();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> BorderMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UStaticMeshComponent> InteriorMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> BoxComponent;

#if WITH_EDITORONLY_DATA
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UArrowComponent> ForwardArrow;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UArrowComponent> BackwardArrow;
#endif

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AStep> PreviousStep;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class AStep> NextStep;

protected:
	virtual void BeginPlay() override;

public:	

	UFUNCTION() 
	virtual void OnArriveOn(APassMasterCharacter* Character) override;

	UFUNCTION()
	virtual void OnPassOver(APassMasterCharacter* Character) override;

	UFUNCTION()
	virtual void OnLeave(APassMasterCharacter* Character) override;

};
