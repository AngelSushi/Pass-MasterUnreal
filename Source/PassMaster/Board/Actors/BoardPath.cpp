#include "BoardPath.h"
#include "Components/SplineComponent.h"

ABoardPath::ABoardPath()
{
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	RootComponent = Spline;
}

void ABoardPath::BeginPlay()
{
	Super::BeginPlay();
	
}


