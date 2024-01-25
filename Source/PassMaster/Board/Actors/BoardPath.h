#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoardPath.generated.h"

UCLASS()
class PASSMASTER_API ABoardPath : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoardPath();

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USplineComponent> Spline;

protected:
	virtual void BeginPlay() override;

};
