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

protected:
	virtual void BeginPlay() override;
};
