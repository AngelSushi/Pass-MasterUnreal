#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BoardPlayerController.generated.h"

class UBoardSubSystem;

UCLASS()
class PASSMASTER_API ABoardPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void OnObjects();

	UFUNCTION(BlueprintCallable)
	void OnDice();

	UFUNCTION(BlueprintCallable)
	void OnMap();

private:
	UPROPERTY()
	UBoardSubSystem* BoardSystem;

};
