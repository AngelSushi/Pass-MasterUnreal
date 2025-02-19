#include "BoardSubSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraActor.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "PassMaster/Board/Core/PassMasterGameMode.h"
#include "PassMaster/Board/Core/PassMasterCharacter.h"
#include "PassMaster/Board/Datas/IsleBoardDataAsset.h"
#include "PassMaster/GameManager.h"
#include "PassMaster/Board/Actors/Dice.h"
#include "PassMaster/Board/Actors/Steps/Step.h"
#include "PassMaster/Board/SubSystems/GridManager.h"

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

	OnEndCoinsAnimEvent.AddDynamic(this, &UBoardSubSystem::OnEndCoinsAnim);

	MainCamera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
}

void UBoardSubSystem::OnSetupBoardFinished(UGameManager* GM, APassMasterGameMode* GameMode) {
	if (!GameMode) {
		return;
	}
	
	BoardAsset = GameMode->BoardAsset;
	GameManager->GameState = BoardAsset->BeginGameState;

	UGridManager* GridManager = GetWorld()->GetSubsystem<UGridManager>();
	GridManager->SetupGrid(this, GetWorld());

	switch (GameManager->GameState) {
		case EGameState::CHOOSE_ORDER:
			OnBeginOrderEvent.Broadcast(GM);
			break;

		case EGameState::PARTYGAME:
			// Order PlayersData by Controller Class

		/*	for (int i = 0; i < GameManager->PlayersData.Num(); i++) {
				FPlayerData& PlayerData = GameManager->PlayersData[i];

				if (APlayerController* PlayerController = Cast<APlayerController>(PlayerData.GetActor()->GetController())) {
					GameManager->PlayersData.Swap(0, GameManager->PlayersData.IndexOfByKey(PlayerData));
				}
			}

			OnEndTransition();
			*/
			break;

		case EGameState::CHOOSE_MINIGAME:
			break;
	}
 }

void UBoardSubSystem::OnBeginOrder(UGameManager* GM) {
	//GM->GameState = EGameState::CHOOSE_ORDER;

	for (int16 i = 0; i < BoardAsset->PlayerCount; i++) {
		//FPlayerData& PlayerData = GM->PlayersData[i];

		//ADice* Dice = GetWorld()->SpawnActor<ADice>(BoardAsset->Dice);
		//Dice->SetActorLocation(PlayerData.GetActor()->GetActorLocation() + FVector::UpVector * 100.f);

		
		/*ADice* Dice = InitDice(PlayerData.GetActor());

		FRotator DiceRotation = UKismetMathLibrary::FindLookAtRotation(Dice->GetActorLocation(), GetMainCamera()->GetActorLocation());
		Dice->SetActorRotation(FRotator(Dice->GetActorRotation().Pitch, DiceRotation.Yaw + 90 + 18, DiceRotation.Roll - 90));


		if (APlayerController* PlayerController = Cast<APlayerController>(PlayerData.GetActor()->GetController())) {
			PlayerController->SetViewTarget(MainCamera);
		}

		PlayerData.GetActor()->bCanJump = true;
		*/
	}
}

void UBoardSubSystem::OnUpdateOrder(APassMasterCharacter* Character, int16 DiceResult) {
	/*if (GameManager->PlayersData.ContainsByPredicate([](const FPlayerData& PlayerData) { return PlayerData.GetOrder() == -1; })) {

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
	*/
}

void UBoardSubSystem::OnEndOrder() {
	OnStartTransitionEvent.Broadcast();
}
 
void UBoardSubSystem::OnStartTransition() {}

void UBoardSubSystem::OnEndTransition() {

	/*for (int16 i = 0; i < GameManager->PlayersData.Num(); i++) {
		
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

	ManagerOfCamera = GameManager->PlayersData.FindByPredicate([](const FPlayerData& PlayerData) {
		return PlayerData.GetActor()->GetController()->IsA(APlayerController::StaticClass());
		})->GetActor();
	
	OnBeginTurn();
	*/
}

APassMasterCharacter* UBoardSubSystem::GetActualPlayer() {
	if (ActualPlayerIndex >= 0) {
		//return GameManager->PlayersData[ActualPlayerIndex].GetActor();
	}

	return nullptr;
}

void UBoardSubSystem::OnBeginTurn() {

	// All General Gestion

	ActualPlayerIndex = 0;
	
	OnBeginTurnEvent.Broadcast();
	
	OnPlayerBeginTurn();
}

void UBoardSubSystem::OnPlayerBeginTurn() {
	OnBeginPlayerTurnEvent.Broadcast(GetActualPlayer(),this);
}

void UBoardSubSystem::OnPlayerEndTurn() {
	OnEndPlayerTurnEvent.Broadcast(GetActualPlayer());

	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Yellow, FString::Printf(TEXT("End Turn For %s"), *GetActualPlayer()->GetActorNameOrLabel()));

	ActualPlayerIndex++;

	if (ActualPlayerIndex >= GameManager->PlayerCount) {
		OnEndTurn();
	}
	else {
		APassMasterCharacter* TurnPlayer = GetActualPlayer();
		
		if (!TurnPlayer) {
			return;
		}

		OnPlayerBeginTurn();
	}

}

void UBoardSubSystem::OnEndTurn() {
	OnEndTurnEvent.Broadcast();
}

void UBoardSubSystem::OnEndCoinsAnim() {
	OnPlayerEndTurn();
}

ADice* UBoardSubSystem::InitDice(APassMasterCharacter* Character) {
	ADice* Dice = GetWorld()->SpawnActor<ADice>(BoardAsset->Dice);
	Dice->SetActorLocation(Character->GetActorLocation() + FVector::UpVector * 150.f);

	return Dice;
}