#include "Coins.h"

ACoins::ACoins(){

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
}

void ACoins::BeginPlay(){
	Super::BeginPlay();
	
}


