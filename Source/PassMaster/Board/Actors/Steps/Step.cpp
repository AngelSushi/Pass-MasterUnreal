#include "Step.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

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

void AStep::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStep::OnArriveOn(APassMasterCharacter* Character) {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("On Arrive On Step"));
}

void AStep::OnPassOver(APassMasterCharacter* Character) {

}

void AStep::OnLeave(APassMasterCharacter* Character) {

}

