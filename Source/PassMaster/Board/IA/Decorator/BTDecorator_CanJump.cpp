#include "BTDecorator_CanJump.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "PassMaster/Board/Core/PassMasterCharacter.h"


UBTDecorator_CanJump::UBTDecorator_CanJump() {
	NodeName = "Can Jump";
}

bool UBTDecorator_CanJump::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return false;
	}

	APassMasterCharacter* Character = Cast<APassMasterCharacter>(BlackboardComp->GetValueAsObject("SelfActor"));

	if (!Character) {
		return false;
	}

	return Character->bCanJump;
}

