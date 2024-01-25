#include "BoardPlayerController.h"

#include "PassMaster/Board/SubSystems/BoardSubSystem.h"
#include "PassMaster/Board/Core/PassMasterCharacter.h"

void ABoardPlayerController::BeginPlay() {
	Super::BeginPlay();

	BoardSystem = GetWorld()->GetSubsystem<UBoardSubSystem>();
}

void ABoardPlayerController::OnObjects() {

}

void ABoardPlayerController::OnDice() {

	bShowMouseCursor = false;

	if (APassMasterCharacter* PCharacter = Cast<APassMasterCharacter>(GetPawn())) {
		PCharacter->bCanJump = true;
		BoardSystem->InitDice(PCharacter);
	}
}

void ABoardPlayerController::OnMap() {

}