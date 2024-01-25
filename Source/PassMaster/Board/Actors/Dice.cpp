#include "Dice.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Engine/GameInstance.h"
#include "Camera/CameraActor.h"

#include "PassMaster/GameManager.h"
#include "PassMaster/Board/Core/PassMasterGameMode.h"
#include "PassMaster/Board/SubSystems/BoardSubSystem.h"
#include "PassMaster/Board/Core/PassMasterCharacter.h"
#include "PassMaster/Board/Core/BoardHUD.h"

ADice::ADice()
{
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Scene;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Scene);
}

void ADice::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this,&ADice::OnBeginOverlap);

	
	HUD = Cast<ABoardHUD>(UGameplayStatics::GetPlayerController(this,0)->GetHUD());

	BoardSystem = GetWorld()->GetSubsystem<UBoardSubSystem>();
	FRotator DiceRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),BoardSystem->GetMainCamera()->GetActorLocation());
	SetActorRotation(FRotator(GetActorRotation().Pitch,DiceRotation.Yaw + 90 + 18,DiceRotation.Roll - 90));

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	GameManager = GameInstance->GetSubsystem<UGameManager>();

}

void ADice::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	Timer += DeltaTime;

	if (Timer >= Speed) {
		Mesh->SetMaterial(0,DiceMaterials[DiceIndex - 1]);
		Timer = 0;
		DiceIndex++;
		
		if (DiceIndex > 6) {
			DiceIndex = 1;
		}

	}
}

void ADice::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (APassMasterCharacter* Character = Cast<APassMasterCharacter>(OtherActor)) {

		// Debug
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
			DiceIndex = 7;
		}

		if (GameManager->GameState == EGameState::CHOOSE_ORDER) {
			BoardSystem->OnUpdateOrderEvent.Broadcast(Character, DiceIndex);
			Destroy();
		}
		else if (GameManager->GameState == EGameState::PARTYGAME) {
			Character->DiceResult = DiceIndex;
			HUD->OnCreateDiceResult();
			HUD->OnRefreshDiceResult(DiceIndex);
			Destroy();

		}
	}
}




