#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BoardSubSystem.generated.h"


UCLASS()
class PASSMASTER_API UBoardSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<class APassMasterCharacter> ActualPlayer;


public:
	UFUNCTION(BlueprintCallable)
	FORCEINLINE class APassMasterCharacter* GetActualPlayer() const { return ActualPlayer; }
};
