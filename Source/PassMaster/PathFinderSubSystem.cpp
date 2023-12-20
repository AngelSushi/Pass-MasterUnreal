#include "PathFinderSubSystem.h"
#include "Components/SplineComponent.h"


void UPathFinderSubSystem::Initialize(FSubsystemCollectionBase& Collection) {
	Super::Initialize(Collection);
}

void UPathFinderSubSystem::Deinitialize() {
	Super::Deinitialize();
}

FTransform UPathFinderSubSystem::GetNextPoint(USplineComponent* Spline,float Distance) {
	return Spline->GetTransformAtDistanceAlongSpline(Distance, ESplineCoordinateSpace::World);
}