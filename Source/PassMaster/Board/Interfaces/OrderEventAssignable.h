#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OrderEventAssignable.generated.h"


UINTERFACE(MinimalAPI)
class UOrderEventAssignable : public UInterface
{
	GENERATED_BODY()

};

class PASSMASTER_API IOrderEventAssignable {

	GENERATED_BODY()

public: 

	UFUNCTION()
	virtual void OnBeginOrder(UGameManager* GM) = 0;

	UFUNCTION()
	virtual void OnUpdateOrder(APassMasterCharacter* Character, int16 OrderResult) = 0;

	UFUNCTION()
	virtual void OnEndOrder() = 0;
};
