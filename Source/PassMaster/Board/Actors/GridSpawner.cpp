#include "GridSpawner.h"
#include "Components/BillboardComponent.h"
#include "Builders/CubeBuilder.h"
#include "Components/BoxComponent.h"
#include "PassMaster/Board/SubSystems/GridManager.h"

AGridSpawner::AGridSpawner(){
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceneComponent;

	First = CreateDefaultSubobject<USceneComponent>(TEXT("First"));
	First->SetupAttachment(RootComponent);

	FirstBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("F_Billboard"));
	FirstBillboard->SetupAttachment(First);

	Second = CreateDefaultSubobject<USceneComponent>(TEXT("Second"));
	Second->SetupAttachment(RootComponent);

	SecondBillboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("S_Billboard"));
	SecondBillboard->SetupAttachment(Second);
}




