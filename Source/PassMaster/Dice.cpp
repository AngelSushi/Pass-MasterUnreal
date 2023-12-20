#include "Dice.h"

ADice::ADice()
{
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Scene;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Scene);
}

void ADice::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentBeginOverlap.AddDynamic(this,&ADice::OnBeginOverlap);
}

void ADice::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Yellow, TEXT("Hit Player"));
}




