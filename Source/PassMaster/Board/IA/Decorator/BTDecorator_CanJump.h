#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_CanJump.generated.h"


UCLASS()
class PASSMASTER_API UBTDecorator_CanJump : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTDecorator_CanJump();

	bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
