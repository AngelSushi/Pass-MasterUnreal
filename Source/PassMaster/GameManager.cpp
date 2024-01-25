#include "GameManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Components/ArrowComponent.h"
#include <limits>

#include "PassMaster/Board/Core/PassMasterGameMode.h"
#include "PassMaster/Board/Datas/IsleBoardDataAsset.h"
#include "PassMaster/Board/SubSystems/BoardSubSystem.h"
#include "PassMaster/Board/Datas/PlayerData.h"
#include "PassMaster/Board/Core/PassMasterCharacter.h"
#include "PassMaster/Board/Actors/Steps/Step.h"
#include "PassMaster/Board/Actors/Steps/DirectionStep.h"

void UGameManager::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
}

void UGameManager::SetupBoard() {
	GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Black, TEXT("Setup Board In Progress"));

	APassMasterGameMode* GameMode = Cast<APassMasterGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	FTimerHandle WaitSendEvent;

	UIsleBoardDataAsset* BoardAsset = GameMode->BoardAsset;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayersStart);
	OrderPlayersStart();

	// OrderSteps
	AStep* FirstStep = FindFirstStep();

	//AssignSteps();

	for (int16 i = 0; i < BoardAsset->PlayerCount; i++) {

		// Init Of Players 
		int16 PlayerIndex = i;
		int16 SpawnIndex = i;

		PlayerIndex = FMath::Clamp(PlayerIndex,0,BoardAsset->PlayersActor.Num() - 1);
		SpawnIndex = FMath::Clamp(SpawnIndex, 0, PlayersStart.Num() - 1);
		

		APassMasterCharacter* NewCharacter = GetWorld()->SpawnActor<APassMasterCharacter>(BoardAsset->PlayersActor[PlayerIndex]);
		NewCharacter->SetActorTransform(PlayersStart[SpawnIndex]->GetActorTransform());
		NewCharacter->SpawnDefaultController();
		UGameplayStatics::SetPlayerControllerID(Cast<APlayerController>(NewCharacter->GetController()), i);
		NewCharacter->bCanJump = false;

		FPlayerData TestData(NewCharacter, FString("User00" + (i+1)),-1, (i + 1), 0, 0);
		PlayersData.Add(TestData);
	}


	GetWorld()->GetTimerManager().SetTimer(WaitSendEvent, [this, GameMode,&WaitSendEvent]() {	
		GEngine->AddOnScreenDebugMessage(-1, 15.F, FColor::Black, TEXT("Setup Board Finished"));
		OnSetupBoardFinished.Broadcast(this,GameMode);
		GetWorld()->GetTimerManager().ClearTimer(WaitSendEvent);
	}, 0.2f, false);
		
}

void UGameManager::OrderPlayersStart() { // Order Player Starts by Position
	PlayersStart.Sort([](const AActor& A, const AActor& B) { return !(A.GetActorLocation().Y < B.GetActorLocation().Y); });
}

void UGameManager::AssignSteps() { // Calculation Of Next And Previous Step For Each Step 
	TArray<AActor*> Steps;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStep::StaticClass(), Steps);	

	/*for (AActor* Actor : Steps) {
		if (AStep* Step = Cast<AStep>(Actor)) {
			TArray<FHitResult> RaycastResults;
			BoxRaycast(Step, Step->GetActorForwardVector(), RaycastResults, false);

			if (RaycastResults.Num() > 0) {
				AStep* NextStep = Cast<AStep>(FindClosestStep(Step, RaycastResults,Step->GetActorForwardVector()));
				Step->NextStep = NextStep;
			}

			RaycastResults.Empty();
			FVector BackwardRotationVec = Step->BackwardArrow->GetComponentRotation().Vector();
			BackwardRotationVec.Normalize();

			BoxRaycast(Step,BackwardRotationVec, RaycastResults, false);
			
			if (RaycastResults.Num() > 0) {
				AStep* PreviousStep = Cast<AStep>(FindClosestStep(Step, RaycastResults,BackwardRotationVec));
				Step->PreviousStep = PreviousStep;
			}
		}
	}*/
}

AStep* UGameManager::FindFirstStep() {
	TArray<FHitResult> HitResults;

	BoxRaycast(PlayersStart[PlayersStart.Num() - 1], PlayersStart[PlayersStart.Num() - 1]->GetActorRightVector() * -1, HitResults, false);
	return Cast<AStep>(HitResults[0].GetActor());
}

void UGameManager::BoxRaycast(AActor* Actor,FVector Direction,TArray<FHitResult>& HitResults,bool bDebug) {
	float Size = 200.F;

	FCollisionShape BoxRaycast = FCollisionShape::MakeBox(FVector(Size, 30.F, 50.f));

	FVector Start = Actor->GetActorLocation();
	FVector End = Start + Direction * Size;
	FVector BoxCenter = (Start + End) * 0.5f;
	FRotator BoxRotation = (End - Start).Rotation();
	
	GetWorld()->SweepMultiByChannel(HitResults, Start, Start, FQuat(BoxRotation), ECC_GameTraceChannel1, BoxRaycast);
	FColor RandomColor = FColor::MakeRandomColor();

	HitResults.RemoveAll([&](const FHitResult& HitResult) {
		return HitResult.GetActor() == Actor;
	});

	if (bDebug) {
		UKismetSystemLibrary::DrawDebugBox(GetWorld(), BoxCenter, FVector(Size, 30.F, 30.F), RandomColor, BoxRotation, 10000.F);
	}
}

AActor* UGameManager::FindClosestStep(AActor* Actor, TArray<FHitResult>& HitResults,FVector Direction) {

	int MaxDistance = std::numeric_limits<int>::max();
	AActor* ClosestStep = nullptr;
	FVector ActorLocation = Actor->GetActorLocation();

	if (HitResults.Num() == 1) {
		FVector StepToResult = HitResults[0].GetActor()->GetActorLocation() - ActorLocation;
		FVector Forward = Direction;

		float Angle = FVector::DotProduct(StepToResult, Forward);

		if (Angle <= 0) { // If Step Is Not In Front Of
			return nullptr;
		}

		return HitResults[0].GetActor();
	}

	for (FHitResult& HitResult : HitResults) {
		FVector TargetResultLocation = HitResult.GetActor()->GetActorLocation();

		// Check If Step Is In Front Of the Actor Or Not 
		FVector StepToResult = TargetResultLocation - ActorLocation;
		FVector Forward = Direction;

		float Angle = FVector::DotProduct(StepToResult, Forward);

		if (Angle <= 0) { // If Step Is Not In Front Of
			continue;
		}

		// Find The Nearest Step In Front Of Actor
		float Distance = FVector::Distance(TargetResultLocation,ActorLocation);

		if (Distance < MaxDistance) {
			MaxDistance = Distance;
			ClosestStep = HitResult.GetActor();
		}
	}

	return ClosestStep;
}

