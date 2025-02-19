#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BoardPlayerController.generated.h"

class UBoardSubSystem;
class UInputMappingContext;
class APassMasterCharacter;
class UInputAction;
struct FInputActionValue;

UCLASS()
class PASSMASTER_API ABoardPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;

	void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UFUNCTION()
	void Jump();

	UFUNCTION()
	void StopJumping();

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void LookAt(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnObjects();

	UFUNCTION(BlueprintCallable)
	void OnDice();

	UFUNCTION(BlueprintCallable)
	void OnMap();

private:
	UPROPERTY()
	TObjectPtr<class APassMasterCharacter> _Character;

	UPROPERTY()
	UBoardSubSystem* BoardSystem;

	UPROPERTY()
	bool _bCanJump;
};
