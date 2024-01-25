#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "../Interfaces/OrderEventAssignable.h"
#include "BoardHUD.generated.h"

class IOrderEventAssignable;

UCLASS()
class PASSMASTER_API ABoardHUD : public AHUD, public IOrderEventAssignable
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UActionWidget> ActionWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UActionWidget> ActionWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDiceResultWidget> DiceResultWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UDiceResultWidget> DiceResultWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UOrderResultWidget> OrderResultWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UOrderResultWidget> OrderResultWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> TransitionWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<class UUserWidget> TransitionWidget;

public:

	void BeginPlay() override;

	UFUNCTION()
	void OnBeginPlayerTurn(APassMasterCharacter* Character, UBoardSubSystem* BoardSubSystem);

	UFUNCTION(BlueprintCallable)
	void OnChooseDice();

	UFUNCTION()
	void OnCreateDiceResult();

	UFUNCTION()
	void OnRefreshDiceResult(float DiceResult);

	UFUNCTION()
	virtual void OnBeginOrder(UGameManager* GM) override;

	UFUNCTION()
	virtual void OnUpdateOrder(APassMasterCharacter* Character, int16 OrderResult) override;

	UFUNCTION()
	virtual void OnEndOrder() override;

	UFUNCTION()
	void OnStartTransition();

private:

	UPROPERTY()
	TObjectPtr<class APlayerController> MainController;

	UPROPERTY()
	TObjectPtr<class UBoardSubSystem> BoardSystem;

	UPROPERTY()
	TArray<UOrderResultWidget*> OrderResultsList;
};
