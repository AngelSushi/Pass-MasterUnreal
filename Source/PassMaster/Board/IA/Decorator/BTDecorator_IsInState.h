	#pragma once

	#include "CoreMinimal.h"
	#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
	#include "BTDecorator_IsInState.generated.h"

	UCLASS()
	class PASSMASTER_API UBTDecorator_IsInState : public UBTDecorator_BlackboardBase
	{
		GENERATED_BODY()
	
	public:
		UBTDecorator_IsInState();

		bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	};
