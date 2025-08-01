// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT(BlueprintType)
struct FEffectEntityProperties
{
	GENERATED_BODY()
	UAbilitySystemComponent* AbilitySystemComponent = nullptr;
	AActor* AvatarActor = nullptr;
	AController* Controller = nullptr;
	ACharacter* Character = nullptr;
};

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()
	FGameplayEffectContextHandle EffectContextHandle;
	FEffectEntityProperties Source;
	FEffectEntityProperties Target;
};

struct FGameplayEffectModCallbackData;

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Vital Attributes")
	FGameplayAttributeData MaxMana;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Vital Attributes")
	FGameplayAttributeData Mana;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	FEffectProperties EffectProperties;

protected:

private:
	static void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
	static void SetEffectEntityProperties(UAbilitySystemComponent* ASC, FEffectEntityProperties& EntityProps);
};
