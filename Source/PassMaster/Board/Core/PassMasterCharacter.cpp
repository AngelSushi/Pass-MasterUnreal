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
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


#include "PassMaster/Board/Interfaces/Collisionnable.h"
#include "PassMaster/Board/SubSystems/BoardSubSystem.h"
#include "PassMaster/GameManager.h"
#include "PassMaster/Board/Actors/Steps/Step.h"
#include "PassMaster/Board/Actors/BoardPath.h"
#include "PassMaster/Board/Actors/Coins.h"
#include "PassMaster/Board/SubSystems/GridManager.h"

#include "Engine/World.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetMathLibrary.h"

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

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); 
	FollowCamera->bUsePawnControlRotation = false;
}

void APassMasterCharacter::BeginPlay()
{ 
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		PController = PlayerController;

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlayerMappingContext, 0);
		}
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this,&APassMasterCharacter::OnBeginOverlap);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &APassMasterCharacter::OnEndOverlap);

	BoardSystem = GetWorld()->GetSubsystem<UBoardSubSystem>();

	BoardSystem->OnBeginPlayerTurnEvent.AddDynamic(this, &APassMasterCharacter::OnPlayerBeginTurn);
	BoardSystem->OnEndPlayerTurnEvent.AddDynamic(this, &APassMasterCharacter::OnPlayerEndTurn);
}

void APassMasterCharacter::OnPlayerBeginTurn(APassMasterCharacter* Character,UBoardSubSystem* BoardSubSystem) {
	if (Character != this) {
		return;
	}

	bIsTurn = true;
	
	if (!GetMesh()->IsVisible()) {
		GetMesh()->SetVisibility(true);
	}

	if (!PController) {
		bCanJump = true;
	}

	FViewTargetTransitionParams Params;
	BoardSystem->GetMainCamera()->GetCameraComponent()->SetActive(true);
	BoardSystem->GetMainCamera()->SetActorLocation(TurnCamPosition->GetComponentLocation());
	BoardSystem->GetMainCamera()->SetActorRelativeRotation(TurnCamPosition->GetRelativeRotation());

	BoardSystem->GetManagerOfCamera()->PController->SetViewTarget(BoardSystem->GetMainCamera(), Params);

	UGridManager* GridManager = GetWorld()->GetSubsystem<UGridManager>();
//	GridManager->CalculateGridForPath(GetActorLocation(), 6);
}

void APassMasterCharacter::OnPlayerEndTurn(APassMasterCharacter* Character) {
	if (Character != this) {
		return;
	}

	bIsTurn = false;
}


void APassMasterCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bIsTurn) {
		if (GetCharacterMovement()->IsMovingOnGround() && DiceResult > 0 && !bIsMoving) {
			bIsMoving = true;

			FViewTargetTransitionParams Params;
			BoardSystem->GetMainCamera()->GetCameraComponent()->SetActive(false);
			
			if (PController) {
				FollowCamera->SetActive(true);
				PController->SetViewTarget(FollowCamera->GetAttachParentActor(), Params);
			}
			else {
				APassMasterCharacter* TurnPlayer = BoardSystem->GetActualPlayer();

				if (!TurnPlayer) {
					return;
				}

				TurnPlayer->FollowCamera->SetActive(true);
				BoardSystem->GetManagerOfCamera()->PController->SetViewTarget(TurnPlayer->FollowCamera->GetAttachParentActor(), Params);

			}
		}

		if (bIsMoving) {
			if (Destination != FVector::Zero()) {
				GEngine->AddOnScreenDebugMessage(-1, 15000.F, FColor::Yellow, FString::Printf(TEXT("Move To %s"), *Destination.ToString()));
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(PController, Destination);
				
			}
		}
	}
}

void APassMasterCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!bIsTurn) {
		return;
	}
	
	if (ICollisionnable* ICollision = Cast<ICollisionnable>(OtherActor)) {
	
		if (!bIsMoving) {
			return;
		}

		DiceResult--;
		ICollision->OnPassOver(this);

		if (DiceResult == 0) {
			ICollision->OnArriveOn(this);
			bIsMoving = false;
		}
	}

	if(OtherActor->IsA(ACoins::StaticClass())) {
		GetWorld()->DestroyActor(OtherActor);
	}
}


void APassMasterCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (!bIsTurn) {
		return;
	}
	
	if (ICollisionnable* ICollision = Cast<ICollisionnable>(OtherActor)) {
		if (!bIsMoving) {
			return;
		}
		
		ICollision->OnLeave(this);
	}
}

void APassMasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APassMasterCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Triggered, this, &APassMasterCharacter::OnTriggerMovement);
		EnhancedInputComponent->BindAction(ClickAction, ETriggerEvent::Completed, this, &APassMasterCharacter::OnCompleteMovement);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APassMasterCharacter::OnTriggerMovement() {
	FHitResult HitResult;

	if (PController->GetHitResultUnderCursor(ECC_Visibility,false,HitResult)) {
		Destination = HitResult.GetActor()->GetActorLocation();

		//FVector UnitWorldVector = Destination;
		//FVector UnitVector = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), UnitWorldVector);

		//GEngine->AddOnScreenDebugMessage(-1, 15000.F, FColor::Yellow, FString::Printf(TEXT("Trigger Movement %s"),*UnitVector.ToString()));

		//AddMovementInput(UnitVector,1.F);
	}
}

void APassMasterCharacter::OnCompleteMovement() {

	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (!PlayerController) {
		return;
	}

	float MouseX, MouseY;
	PlayerController->GetMousePosition(MouseX, MouseY);

	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY,WorldLocation,WorldDirection);
	GEngine->AddOnScreenDebugMessage(-1, 15000.F, FColor::Orange, FString::Printf(TEXT("Mouse Position %s"), *WorldLocation.ToString()));
	
	TArray<FHitResult> Result;
	FVector Start = WorldLocation;
	FVector End = Start + WorldDirection * 10000;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceMultiByChannel(Result, Start, End, ECC_Visibility, Params)) {
		GEngine->AddOnScreenDebugMessage(-1, 15000.F, FColor::Magenta, FString::Printf(TEXT("Result %d"), Result.Num()));
		for (FHitResult R : Result) {
			GEngine->AddOnScreenDebugMessage(-1, 15000.F, FColor::Orange, FString::Printf(TEXT("Name %s"), *R.GetActor()->GetActorNameOrLabel()));

		}
	}
	

}
