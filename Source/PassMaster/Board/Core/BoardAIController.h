#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BoardAIController.generated.h"


UCLASS()
class PASSMASTER_API ABoardAIController : public AAIController
{
	GENERATED_BODY()

	ABoardAIController();

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBehaviorTree> BehaviorTree;

	UPROPERTY(EditAnywhere)
	UBlackboardComponent* BlackboardComponent;

protected:
	virtual void BeginPlay() override;
};
