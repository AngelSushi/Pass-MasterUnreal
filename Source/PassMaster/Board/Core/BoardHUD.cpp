#include "BoardHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PassMasterCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HorizontalBox.h"

#include "PassMaster/Board/UMG/ActionWidget.h"
#include "PassMaster/Board/SubSystems/BoardSubSystem.h"
#include "PassMaster/Board/UMG/DiceResultWidget.h"
#include "PassMaster/Board/UMG/OrderListWidget.h"
#include "PassMaster/Board/UMG/OrderResultWidget.h"

void ABoardHUD::BeginPlay() {
	Super::BeginPlay();

	BoardSystem = GetWorld()->GetSubsystem<UBoardSubSystem>();
	BoardSystem->OnBeginPlayerTurnEvent.AddDynamic(this, &ABoardHUD::OnBeginPlayerTurn);

	BoardSystem->OnBeginOrderEvent.AddDynamic(this, &ABoardHUD::OnBeginOrder);
	BoardSystem->OnUpdateOrderEvent.AddDynamic(this, &ABoardHUD::OnUpdateOrder);


	BoardSystem->OnStartTransitionEvent.AddDynamic(this, &ABoardHUD::OnStartTransition);
}

void ABoardHUD::OnBeginPlayerTurn(APassMasterCharacter* Character,UBoardSubSystem* BoardSubSystem) {

	if (ActionWidgetClass) {
		ActionWidget = CreateWidget<UActionWidget>(GetWorld(), ActionWidgetClass);
		ActionWidget->AddToViewport();

		if (APlayerController* PController = Cast<APlayerController>(Character->GetController())) {
			PController->bShowMouseCursor = true;
			//UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(PController, ActionWidget, EMouseLockMode::DoNotLock);
		}
	}
	
}


void ABoardHUD::OnChooseDice() {
	if (!BoardSystem->GetActualPlayer()) {
		return;
	}

	if (APlayerController* PController = Cast<APlayerController>(BoardSystem->GetActualPlayer()->GetController())) {
		//UWidgetBlueprintLibrary::SetInputMode_GameOnly(PController);
	}
	

	ActionWidget->RemoveFromViewport();

}

void ABoardHUD::OnCreateDiceResult() {
	if (DiceResultWidgetClass) {
		DiceResultWidget = CreateWidget<UDiceResultWidget>(GetWorld(), DiceResultWidgetClass);
		DiceResultWidget->AddToViewport();
	}
}
void ABoardHUD::OnBeginOrder(UGameManager* Manager) {
	MainController = Cast<APlayerController>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerController::StaticClass()));
	OrderResultsList.Empty();
}

void ABoardHUD::OnUpdateOrder(APassMasterCharacter* Character, int16 OrderResult) {
	if (OrderResultWidgetClass) {
		OrderResultWidget = CreateWidget<UOrderResultWidget>(GetWorld(),OrderResultWidgetClass);
		
		if (!OrderResultWidget) {
			return;
		}

		OrderResultWidget->OrderResult = OrderResult;


		if (MainController) {
			FVector2D ScreenPosition;
			if (UGameplayStatics::ProjectWorldToScreen(MainController, Character->GetActorLocation(), ScreenPosition)) {
				ScreenPosition.X -= (120.f / 2.f); // Font Size Of Text / 2
				ScreenPosition.Y = GEngine->GameViewport->Viewport->GetSizeXY().Y * 0.1f;

				OrderResultWidget->SetPositionInViewport(ScreenPosition);
				OrderResultWidget->AddToViewport();
				OrderResultsList.Add(OrderResultWidget);
			}
		}
	}
}

void ABoardHUD::OnEndOrder() {}


void ABoardHUD::OnStartTransition() {
	for(UOrderResultWidget* OrderResultInstance : OrderResultsList) {
		OrderResultInstance->RemoveFromViewport();
	}

	if (TransitionWidgetClass) {
		TransitionWidget = CreateWidget<UUserWidget>(GetWorld(), TransitionWidgetClass);
		TransitionWidget->AddToViewport();
	}
}

void ABoardHUD::OnRefreshDiceResult(float DiceResult) {
	DiceResultWidget->DiceResult = FString::FromInt(DiceResult);
}