#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Jump.generated.h"


UCLASS()
class PASSMASTER_API UBTTask_Jump : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UBTTask_Jump();
	
public: 
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
