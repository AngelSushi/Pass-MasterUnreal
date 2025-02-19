#include "NormalStep.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

#include "PassMaster/Board/Core/BoardHUD.h"
#include "PassMaster/Board/Core/PassMasterCharacter.h"
#include "PassMaster/Board/SubSystems/BoardSubSystem.h"
#include "PassMaster/GameManager.h"
#include "PassMaster/Board/Datas/IsleBoardDataAsset.h"
#include "PassMaster/Board/Actors/Coins.h"

ANormalStep::ANormalStep() {}

void ANormalStep::BeginPlay() {
	Super::BeginPlay();
}

void ANormalStep::OnPassOver(APassMasterCharacter* Character) {
	Super::OnPassOver(Character);
}

void ANormalStep::OnArriveOn(APassMasterCharacter* Character) {
	Super::OnArriveOn(Character);
	FTimerHandle Timer;

	GetWorld()->GetTimerManager().SetTimer(Timer, [this,Character]() {
		//BoardSystem->OnStartCoinsAnimEvent.Broadcast(Character,Coins);

		// Spawn Anim	
		// ZOom Out 

		for (int i = 0; i < Coins; i++) {
			FTimerHandle CoinTimer;
			GetWorld()->GetTimerManager().SetTimer(CoinTimer, [this, Character]() {

				ACoins* Coin = GetWorld()->SpawnActor<ACoins>(BoardSystem->GetBoardAsset()->Coins);
				Coin->SetActorScale3D(FVector(0.3f, 0.3f, 0.3f));
				Coin->SetActorLocation(Character->GetActorLocation() + FVector(0.F, 0.F, 200.F));

			}, 0.15f, false);
		}


		/*FPlayerData* PlayerData = BoardSystem->GameManager->PlayersData.FindByPredicate([Character](FPlayerData& Data) {
			return Data.GetActor() == Character;
		});

		if (!PlayerData) {
			return;
		}

		PlayerData->AddCoins(Coins);
		BoardSystem->GameManager->UpdateDatas();
		*/


		//BoardSystem->OnEndCoinsAnimEvent.Broadcast();
	}, 1.f, false);
	
}