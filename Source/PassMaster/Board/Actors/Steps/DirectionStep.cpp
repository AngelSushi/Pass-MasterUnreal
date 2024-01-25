#include "DirectionStep.h"
#include "Components/ArrowComponent.h"

ADirectionStep::ADirectionStep() {
#if WITH_EDITOR

	Second = CreateDefaultSubobject<UArrowComponent>(TEXT("Second"));
	Second->SetupAttachment(RootComponent);

	Third = CreateDefaultSubobject<UArrowComponent>(TEXT("Third"));
	Third->SetupAttachment(RootComponent);

	Fourth = CreateDefaultSubobject<UArrowComponent>(TEXT("Fourth"));
	Fourth->SetupAttachment(RootComponent);

	Second->SetWorldRotation(FVector::DownVector.Rotation());
	Third->SetWorldRotation(FVector::DownVector.Rotation());
	Fourth->SetWorldRotation(FVector::DownVector.Rotation());


#endif

}

void ADirectionStep::BeginPlay() {
	Super::BeginPlay();
}