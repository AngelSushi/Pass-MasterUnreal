// Copyright Epic Games, Inc. All Rights Reserved.

#include "PassMasterCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraActor.h"


#include "PassMaster/Board/Interfaces/Collisionnable.h"
#include "PassMaster/Board/SubSystems/PathFinderSubSystem.h"
#include "PassMaster/Board/Actors/BoardPath.h"
#include "PassMaster/Board/SubSystems/BoardSubSystem.h"
#include "PassMaster/GameManager.h"

#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APassMasterCharacter

APassMasterCharacter::APassMasterCharacter()
{

	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(RootComponent); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; 

	StepCamPos = CreateDefaultSubobject<USceneComponent>(TEXT("Step Cam Pos"));
	StepCamPos->SetupAttachment(RootComponent);
}

void APassMasterCharacter::BeginPlay()
{ 
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PController = PlayerController;

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&APassMasterCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APassMasterCharacter::OnEndOverlap);

	BoardSystem = GetWorld()->GetSubsystem<UBoardSubSystem>();

	BoardSystem->OnBeginPlayerTurnEvent.AddDynamic(this, &APassMasterCharacter::OnPlayerBeginTurn);
}

void APassMasterCharacter::OnPlayerBeginTurn(APassMasterCharacter* Character,UBoardSubSystem* BoardSubSystem) {
	if (Character != this) {
		return;
	}
	
	if (!GetMesh()->IsVisible()) {
		GetMesh()->SetVisibility(true);
	}

	FollowCamera->Deactivate();
	
	BoardSystem->GetMainCamera()->SetActorLocation(StepCamPos->GetComponentLocation());

	// Rotate Camera To Face Player 
	FRotator CameraRotation = UKismetMathLibrary::FindLookAtRotation(BoardSystem->GetMainCamera()->GetActorLocation(), GetActorLocation());
	FRotator OriginalRotation = BoardSystem->GetMainCamera()->GetActorRotation();
	BoardSystem->GetMainCamera()->SetActorRotation(FRotator(OriginalRotation.Pitch,CameraRotation.Yaw,OriginalRotation.Roll));
}


void APassMasterCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsMovingOnGround() && DiceResult > 0 && !bIsMoving) {
		bIsMoving = true;
		FollowCamera->Activate();
	}

	if (bIsMoving) {
		SplineDistance += DeltaTime * Speed;

		FTransform NextTransform = UPathFinderSubSystem::GetNextPoint(BoardPath->Spline,SplineDistance);

		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(FVector(GetActorLocation().X,GetActorLocation().Y,0.f)
			,FVector(NextTransform.GetLocation().X,NextTransform.GetLocation().Y,0.F));

		SetActorLocation(FVector(NextTransform.GetLocation().X, NextTransform.GetLocation().Y,GetActorLocation().Z));
		SetActorRotation(Rotation);
	}
}

void APassMasterCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (ICollisionnable* ICollision = Cast<ICollisionnable>(OtherActor)) {
	
		if (!bIsMoving) {
			return;
		}

		if (DiceResult > 0) {
			DiceResult--;
			ICollision->OnPassOver(this);
		}
		else {
			ICollision->OnArriveOn(this);
			bIsMoving = false;
		}
	}
}


void APassMasterCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (ICollisionnable* ICollision = Cast<ICollisionnable>(OtherActor)) {
		if (!bIsMoving) {
			return;
		}
		
		ICollision->OnLeave(this);
	}
}

void APassMasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APassMasterCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		//EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APassMasterCharacter::Move);

		// Looking
		//EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APassMasterCharacter::Look);

		// Follow
		EnhancedInputComponent->BindAction(StartFollowAction, ETriggerEvent::Started, this,&APassMasterCharacter::StartFollow);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APassMasterCharacter::Jump() {
	if (bCanJump) {
		Super::Jump();
		bCanJump = false;
	}
}

void APassMasterCharacter::StartFollow() {
	//bIsMoving = !bIsMoving;
}

void APassMasterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(1, -15.F, FColor::Yellow, TEXT("Je suis ici en moove"));

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

	}

	
}
void APassMasterCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}