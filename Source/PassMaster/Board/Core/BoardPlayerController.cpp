#include "BoardPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraActor.h"

#include "PassMaster/Board/SubSystems/BoardSubSystem.h"
#include "PassMaster/Board/Core/PassMasterCharacter.h"
#include "PassMaster/Board/Actors/Dice.h"
#include "PassMaster/Board/SubSystems/BoardSubSystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ABoardPlayerController::BeginPlay() {
	Super::BeginPlay();

	BoardSystem = GetWorld()->GetSubsystem<UBoardSubSystem>();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		Subsystem->AddMappingContext(PlayerMappingContext, 0);
	}

	if (APassMasterCharacter* PCharacter = Cast<APassMasterCharacter>(GetPawn())) {
		_Character = PCharacter;
	}

	_bCanJump = true;
}

void ABoardPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ABoardPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ABoardPlayerController::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABoardPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABoardPlayerController::LookAt);
	}

}

void ABoardPlayerController::Jump() {
	if (!_Character) 
		return;

	if (_bCanJump) {
		_Character->Jump();
		_bCanJump = false;
	}
}

void ABoardPlayerController::StopJumping() {
	if (_Character)
		_Character->StopJumping();
}

void ABoardPlayerController::Move(const FInputActionValue& Value) {
	const FVector2D MovementVector = Value.Get<FVector2D>();

	//GetPawn()->AddMovementInput(FVector::ForwardVector, MovementVector.Y);
	//GetPawn()->AddMovementInput(FVector::RightVector, MovementVector.X);
}

void ABoardPlayerController::LookAt(const FInputActionValue& Value) {
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	//AddYawInput(LookAxisVector.X);
	//AddPitchInput(LookAxisVector.Y);
}

void ABoardPlayerController::OnObjects() {

}

void ABoardPlayerController::OnDice() {

	bShowMouseCursor = false;

	if (APassMasterCharacter* PCharacter = Cast<APassMasterCharacter>(GetPawn())) {
		PCharacter->bCanJump = true;
		ADice* Dice = BoardSystem->InitDice(PCharacter);
		FRotator DiceRotation = UKismetMathLibrary::FindLookAtRotation(Dice->GetActorLocation(), BoardSystem->GetMainCamera()->GetActorLocation());
		Dice->SetActorRotation(FRotator(Dice->GetActorRotation().Pitch, DiceRotation.Yaw -180, DiceRotation.Roll - 90));
	}
}

void ABoardPlayerController::OnMap() {

}
