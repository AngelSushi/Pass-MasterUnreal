// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PassMasterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UBoardSubSystem;
class ABoardPath;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
UCLASS(config=Game)
class APassMasterCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* PlayerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ClickAction;

public:
	APassMasterCharacter();

	UPROPERTY(EditAnywhere)
	float Speed;

	UPROPERTY(EditAnywhere)
	float DiceResult;

	UPROPERTY(EditAnywhere)
	bool bIsMoving;

	UPROPERTY(EditAnywhere)
	bool bCanJump;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UTexture2D> UIIcon;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> TurnCamPosition;

private:

	UBoardSubSystem* BoardSystem;

	UPROPERTY(EditAnywhere)
	bool bIsTurn;

	UPROPERTY()
	TObjectPtr<class APlayerController> PController;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class AStep> CurrentStep;

	UPROPERTY()
	FVector Destination;


protected:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay();

public:

	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
	void OnPlayerBeginTurn(APassMasterCharacter* Character,UBoardSubSystem* BoardSubSystem);
	
	UFUNCTION()
	void OnPlayerEndTurn(APassMasterCharacter* Character);

	UFUNCTION()
	void OnTriggerMovement();

	UFUNCTION()
	void OnCompleteMovement();

	TObjectPtr<class AStep>& GetCurrentStep() { return CurrentStep; }

	void SetCurrentStep(AStep* Step) { CurrentStep = Step; }

	UFUNCTION()
	bool IsTurn() { return bIsTurn; }

	UFUNCTION(BlueprintCallable)
	UTexture2D* GetUIIcon() { return UIIcon; }
};

