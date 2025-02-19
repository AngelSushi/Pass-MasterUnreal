#include "PathFinderSubSystem.h"
#include "Components/SplineComponent.h"	
#include "Kismet/GameplayStatics.h"

#include "PassMaster/Board/Core/PassMasterCharacter.h"
#include "PassMaster/Board/Actors/Steps/Step.h"
#include "PassMaster/Board/Actors/BoardPath.h"


// SUPPRIMER

void UPathFinderSubSystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
}

void UPathFinderSubSystem::Deinitialize() {
	Super::Deinitialize();
}


