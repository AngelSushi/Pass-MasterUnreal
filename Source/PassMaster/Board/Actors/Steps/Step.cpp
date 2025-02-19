#include "Step.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

#include "PassMaster/Board/Core/PassMasterCharacter.h"
#include "PassMaster/Board/SubSystems/BoardSubSystem.h"

AStep::AStep() {
	BorderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Border Mesh"));
	RootComponent = BorderMesh;

	InteriorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interior Mesh"));
	InteriorMesh->SetupAttachment(BorderMesh);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(BorderMesh);

#if WITH_EDITOR

	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	ForwardArrow->SetupAttachment(BorderMesh);
	ForwardArrow->ArrowColor = FColor::Green;

	BackwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("BackwardArrow"));
	BackwardArrow->SetupAttachment(BorderMesh);
	BackwardArrow->ArrowColor = FColor::Red;

	BackwardArrow->SetWorldRotation((GetActorForwardVector() * -1).Rotation());
#endif
}

void AStep::BeginPlay() {
	Super::BeginPlay();

	BoardSystem = GetWorld()->GetSubsystem<UBoardSubSystem>();
}

void AStep::OnArriveOn(APassMasterCharacter* Character) {
	BoardSystem->OnArriveOnStepEvent.Broadcast(Character, this);
}

void AStep::OnPassOver(APassMasterCharacter* Character) {
	BoardSystem->OnPassOverStepEvent.Broadcast(Character, this);
}

void AStep::OnLeave(APassMasterCharacter* Character) {
	BoardSystem->OnLeaveStepEvent.Broadcast(Character, this);
}

