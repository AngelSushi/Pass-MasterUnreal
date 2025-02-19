#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_InitDice.generated.h"


UCLASS()
class PASSMASTER_API UBTTask_InitDice : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_InitDice();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
