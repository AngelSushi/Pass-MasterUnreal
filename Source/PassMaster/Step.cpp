#include "Step.h"

AStep::AStep()
{
	PrimaryActorTick.bCanEverTick = true;

	BorderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Border Mesh"));
	RootComponent = BorderMesh;

	InteriorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Interior Mesh"));
	InteriorMesh->SetupAttachment(BorderMesh);

}

void AStep::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStep::OnArriveOn(APassMasterCharacter* Character) {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("On Arrive On Step"));
}

void AStep::OnPassOver(APassMasterCharacter* Character) {

}

void AStep::OnLeave(APassMasterCharacter* Character) {

}

