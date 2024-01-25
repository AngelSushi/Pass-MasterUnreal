#pragma once

#include "CoreMinimal.h"
#include "Step.h"
#include "DirectionStep.generated.h"


UCLASS()
class PASSMASTER_API ADirectionStep : public AStep
{
	GENERATED_BODY()
	
public:
	ADirectionStep();

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UArrowComponent> Second;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UArrowComponent> Third;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UArrowComponent> Fourth;
	
#endif

	UPROPERTY(EditAnywhere)
	bool bUseSecond;

	UPROPERTY(EditAnywhere)
	bool bUseThird;

	UPROPERTY(EditAnywhere)
	bool bUseFourth;

protected:
	virtual void BeginPlay() override;
};
