#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PathFinderSubSystem.generated.h"

class USplineComponent;

UCLASS()
class PASSMASTER_API UPathFinderSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	

public: 
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void Deinitialize() override;

	static FTransform GetNextPoint(USplineComponent* Spline,float Distance);
};
