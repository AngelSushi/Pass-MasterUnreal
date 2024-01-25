#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrderResultWidget.generated.h"

UCLASS()
class PASSMASTER_API UOrderResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int16 OrderResult;

	UFUNCTION(BlueprintCallable)
	FString GetOrderResult();
};
