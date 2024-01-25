#include "BoardSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"

#include "PassMaster/Board/Core/PassMasterGameMode.h"
#include "PassMaster/Board/Core/PassMasterCharacter.h"
#include "PassMaster/Board/Datas/IsleBoardDataAsset.h"
#include "PassMaster/GameManager.h"
#include "PassMaster/Board/Datas/PlayerData.h"
#include "PassMaster/Board/Actors/Dice.h"

void UBoardSubSystem::Initialize(FSubsystemCollectionBase& CollectionBase) {
	Super::Initialize(CollectionBase);
}

void UBoardSubSystem::Deinitialize() {
	Super::Deinitialize();
}

void UBoardSubSystem::OnWorldBeginPlay(UWorld& InWorld) {
	Super::OnWorldBeginPlay(InWorld);

	UGameInstance* GameInstance = InWorld.GetGameInstance();

	
	GameManager = GameInstance->GetSubsystem<UGameManager>();

	FTimerHandle Timer;

	GetWorld()->GetTimerManager().SetTimer(Timer, [&]() { GameManager->SetupBoard(); }, 0.2f, false);

	GameManager->OnSetupBoardFinished.AddDynamic(this, &UBoardSubSystem::OnSetupBoardFinished);

	OnBeginOrderEvent.AddDynamic(this, &UBoardSubSystem::OnBeginOrder);
	OnUpdateOrderEvent.AddDynamic(this, &UBoardSubSystem::OnUpdateOrder);
	OnEndOrderEvent.AddDynamic(this, &UBoardSubSystem::OnEndOrder);

	OnEndTransitionEvent.AddDynamic(this, &UBoardSubSystem::OnEndTransition);

	MainCamera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
}

void UBoardSubSystem::OnSetupBoardFinished(UGameManager* GM, APassMasterGameMode* GameMode) {
	if (!GameMode) {
		return;
	}
	
	BoardAsset = GameMode->BoardAsset;
	GameManager->GameState = BoardAsset->BeginGameState;

	switch (GameManager->GameState) {
		case EGameState::CHOOSE_ORDER:
			OnBeginOrderEvent.Broadcast(GM);
			break;

		case EGameState::PARTYGAME:
			//OnEndTransition();
			break;

		case EGameState::CHOOSE_MINIGAME:
			break;
	}
 }

void UBoardSubSystem::OnBeginOrder(UGameManager* GM) {
	//GM->GameState = EGameState::CHOOSE_ORDER;

	for (int16 i = 0; i < BoardAsset->PlayerCount; i++) {
		FPlayerData& PlayerData = GM->PlayersData[i];

		ADice* Dice = GetWorld()->SpawnActor<ADice>(BoardAsset->Dice);
		Dice->SetActorLocation(PlayerData.GetActor()->GetActorLocation() + FVector::UpVector * 100.f);

		if (APlayerController* PlayerController = Cast<APlayerController>(PlayerData.GetActor()->GetController())) {
			PlayerController->SetViewTarget(MainCamera);
		}

		PlayerData.GetActor()->bCanJump = true;
	}
}

void UBoardSubSystem::OnUpdateOrder(APassMasterCharacter* Character, int16 DiceResult) {
	if (GameManager->PlayersData.ContainsByPredicate([](const FPlayerData& PlayerData) { return PlayerData.GetOrder() == -1; })) { 

		FPlayerData* PlayerData = GameManager->PlayersData.FindByPredicate([Character, DiceResult](FPlayerData& PlayerData) { return PlayerData.GetActor() == Character; });

		if (PlayerData) {
			PlayerData->SetOrder(DiceResult);
		}
		GameManager->PlayersData.Sort([](const FPlayerData& A, const FPlayerData& B) { return !(A.GetOrder() < B.GetOrder()); });

		if (!GameManager->PlayersData.ContainsByPredicate([](const FPlayerData& PlayerData) { return PlayerData.GetOrder() == -1; })) {

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
				OnEndOrderEvent.Broadcast();
			}, 1.f, false);
		}
		
	}	
	
}

void UBoardSubSystem::OnEndOrder() {
	OnStartTransitionEvent.Broadcast();
}
 
void UBoardSubSystem::OnStartTransition() {}

void UBoardSubSystem::OnEndTransition() {

	for (int16 i = 0; i < GameManager->PlayersData.Num(); i++) {
		
		int16 SpawnIndex = i;
		SpawnIndex = FMath::Clamp(SpawnIndex, 0,GameManager->PlayersStart.Num() - 1);

		FPlayerData& PlayerData = GameManager->PlayersData[i];

		// Set New Actor Location
		PlayerData.GetActor()->SetActorLocation(GameManager->PlayersStart[GameManager->PlayersStart.Num() - 1 - i]->GetActorLocation());


		// Rotate Player
		FRotator PlayerActualRotation = PlayerData.GetActor()->GetActorRotation();
		FVector NewRotationVector = PlayerActualRotation.RotateVector(FVector(0,90,0));
		PlayerData.GetActor()->SetActorRotation(FRotator(NewRotationVector.ToOrientationRotator()));

		PlayerData.GetActor()->GetMesh()->SetVisibility(false);

	}
	
	OnBeginTurn();
}

APassMasterCharacter* UBoardSubSystem::GetActualPlayer() {
	return GameManager->PlayersData[ActualPlayerIndex].GetActor();
}

void UBoardSubSystem::OnBeginTurn() {

	// All General Gestion

	ActualPlayerIndex = -1;
	
	OnBeginTurnEvent.Broadcast();
	
	OnPlayerBeginTurn();
}

void UBoardSubSystem::OnPlayerBeginTurn() {
	ActualPlayerIndex++;
	FMath::Clamp(ActualPlayerIndex, 0, GameManager->PlayerCount - 1);

	OnBeginPlayerTurnEvent.Broadcast(GetActualPlayer(),this);
}

void UBoardSubSystem::OnPlayerEndTurn() {
	OnEndPlayerTurnEvent.Broadcast(GetActualPlayer());
}

void UBoardSubSystem::OnEndTurn() {
	OnEndTurnEvent.Broadcast();
}

void UBoardSubSystem::InitDice(APassMasterCharacter* Character) {
	ADice* Dice = GetWorld()->SpawnActor<ADice>(BoardAsset->Dice);
	Dice->SetActorLocation(Character->GetActorLocation() + FVector::UpVector * 100.f);
}