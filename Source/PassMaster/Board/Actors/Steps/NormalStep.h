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

	virtual void OnArriveOn(APassMasterCharacter* Character) override;
	virtual void OnPassOver(APassMasterCharacter* Character) override;

private:
	UPROPERTY()
	TObjectPtr<class ABoardHUD> HUD;
};
