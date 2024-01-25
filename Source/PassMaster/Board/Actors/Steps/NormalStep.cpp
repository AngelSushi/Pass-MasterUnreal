#include "NormalStep.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"

#include "PassMaster/Board/Core/BoardHUD.h"
#include "PassMaster/Board/Core/PassMasterCharacter.h"

ANormalStep::ANormalStep() {}

void ANormalStep::BeginPlay()
{
	Super::BeginPlay();
	HUD = Cast<ABoardHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
}

void ANormalStep::OnPassOver(APassMasterCharacter* Character) {
	Super::OnPassOver(Character);

	HUD->OnRefreshDiceResult(Character->DiceResult);
}

void ANormalStep::OnArriveOn(APassMasterCharacter* Character) {
	Super::OnArriveOn(Character);

	HUD->OnRefreshDiceResult(Character->DiceResult);
}