#include "BTDecorator_IsInState.h"
#include "PassMaster/GameManager.h"

UBTDecorator_IsInState::UBTDecorator_IsInState() {
	NodeName = "Is In State Choose Order";
}

bool UBTDecorator_IsInState::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UGameManager* GameManager = GetWorld()->GetGameInstance()->GetSubsystem<UGameManager>();

	if (State == GameManager->GameState) {
		return true;
	}
	else {
		return false;
	}

}