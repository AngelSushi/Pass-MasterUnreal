#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoinsWidget.generated.h"


UCLASS()
class PASSMASTER_API UCoinsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FString GetCoins();

	FString Coins;
};
