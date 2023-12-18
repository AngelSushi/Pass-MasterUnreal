#pragma once

#include "CoreMinimal.h"
#include "Step.h"
#include "NormalStep.generated.h"


UCLASS()
class PASSMASTER_API ANormalStep : public AStep
{
	GENERATED_BODY()

public:
	ANormalStep();

	UPROPERTY(EditAnywhere)
	uint16 Coins;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
