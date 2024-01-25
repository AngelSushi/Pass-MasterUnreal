#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OrderListWidget.generated.h"

class UHorizontalBox;

UCLASS()
class PASSMASTER_API UOrderListWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	UHorizontalBox* Content;
};
