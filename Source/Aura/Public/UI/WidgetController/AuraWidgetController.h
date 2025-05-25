// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"
class AAuraPlayerState;
class AAuraPlayerController;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;

UCLASS()
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AAuraPlayerState> AAuraPlayerState;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<AAuraPlayerController> PlayerController;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAuraAttributeSet> AttributeSet;

protected:
private:
};
