#include "BTTask_Jump.h"
#include "PassMaster/Board/Core/PassMasterCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"


UBTTask_Jump::UBTTask_Jump() {
	NodeName = "Jump";
}


EBTNodeResult::Type UBTTask_Jump::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APassMasterCharacter* Character = Cast<APassMasterCharacter>(BlackboardComp->GetValueAsObject("SelfActor"));

	if (!Character) {
		return EBTNodeResult::Failed;
	}

	Character->Jump();

	return EBTNodeResult::Succeeded;
}