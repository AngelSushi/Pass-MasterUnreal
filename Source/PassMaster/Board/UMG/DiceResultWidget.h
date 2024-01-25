#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DiceResultWidget.generated.h"


UCLASS()
class PASSMASTER_API UDiceResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FString GetDiceResult();

	FString DiceResult;
	
};
