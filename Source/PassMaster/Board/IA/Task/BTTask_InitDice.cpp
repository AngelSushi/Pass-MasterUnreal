#include "BTTask_InitDice.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraActor.h"

#include "PassMaster/Board/SubSystems/BoardSubSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "PassMaster/Board/Core/PassMasterCharacter.h"
#include "PassMaster/Board/Actors/Dice.h"

UBTTask_InitDice::UBTTask_InitDice() {
	NodeName = "Init Dice";
}

EBTNodeResult::Type UBTTask_InitDice::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::AbortTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr){
		return EBTNodeResult::Failed;
	}

	APassMasterCharacter* Character = Cast<APassMasterCharacter>(BlackboardComp->GetValueAsObject("SelfActor"));
	UBoardSubSystem* BoardSystem = GetWorld()->GetSubsystem<UBoardSubSystem>();

	if (!Character) {
		return EBTNodeResult::Failed;
	}

	ADice* Dice = BoardSystem->InitDice(Character);
	FRotator DiceRotation = UKismetMathLibrary::FindLookAtRotation(Dice->GetActorLocation(), BoardSystem->GetMainCamera()->GetActorLocation());
	Dice->SetActorRotation(FRotator(Dice->GetActorRotation().Pitch, DiceRotation.Yaw - 180, DiceRotation.Roll - 90));

	return EBTNodeResult::Succeeded;

}