#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include  "AbilitySystem/AuraAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = Cast<UAuraAttributeSet>(AttributeSet);
	if (AS)
	{
		OnHealthChanged.Broadcast(AS->GetHealth());
		OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
		OnManaChanged.Broadcast(AS->GetMana());
		OnMaxManaChanged.Broadcast(AS->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetHealthAttribute()).AddUObject(
		this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxHealthAttribute()).AddUObject(
		this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetManaAttribute()).AddUObject(
		this, &UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAuraAttributeSet::GetMaxManaAttribute()).AddUObject(
		this, &UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
