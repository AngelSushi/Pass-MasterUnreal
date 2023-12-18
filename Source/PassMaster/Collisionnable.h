#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Collisionnable.generated.h"

class APassMasterCharacter;

UINTERFACE(MinimalAPI)
class UCollisionnable : public UInterface
{
	GENERATED_BODY()
	
};

class PASSMASTER_API ICollisionnable {
	
	GENERATED_BODY()


public:
	UFUNCTION()
	virtual void OnArriveOn(APassMasterCharacter* Character) = 0;

	UFUNCTION()
	virtual void OnPassOver(APassMasterCharacter* Character) = 0;

	UFUNCTION()
	virtual void OnLeave(APassMasterCharacter* Character) = 0;
};

