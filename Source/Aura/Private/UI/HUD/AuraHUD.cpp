#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Param)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(Param);
		OverlayWidgetController->BindCallbacksToDependencies();
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass is not set!"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass is not set!"));

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	check(Widget)
	OverlayWidget = CastChecked<UAuraUserWidget>(Widget);
	FWidgetControllerParams WidgetParams(PC, PS, ASC, AS);
	UOverlayWidgetController* InWidgetController = GetOverlayWidgetController(WidgetParams);
	OverlayWidget->SetWidgetController(InWidgetController);
	InWidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport();
}
