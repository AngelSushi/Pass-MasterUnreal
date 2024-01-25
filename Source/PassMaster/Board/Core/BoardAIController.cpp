#include "BoardAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ABoardAIController::ABoardAIController() {

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
}

void ABoardAIController::BeginPlay() {
	Super::BeginPlay();

	if (BehaviorTree) {
		RunBehaviorTree(BehaviorTree);

		BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
		UseBlackboard(BehaviorTree->BlackboardAsset, BlackboardComponent);
	}
}	