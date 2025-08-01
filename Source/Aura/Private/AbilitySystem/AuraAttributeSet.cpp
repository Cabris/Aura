// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "GameplayEffect.h"
#include "GameFramework/Character.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	InitHealth(20.0f);
	InitMaxHealth(100.0f);
	InitMana(10.0f);
	InitMaxMana(100.0f);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxHealth, OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Mana, OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, MaxMana, OldMaxMana);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	UE_LOG(LogTemp, Warning, TEXT("UAuraAttributeSet::PreAttributeChange: %s, NewValue: %f"),*Attribute.AttributeName, NewValue);
}

void UAuraAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
	UE_LOG(LogTemp, Warning, TEXT("UAuraAttributeSet::PreAttributeBaseChange: %s, NewValue: %f"),*Attribute.AttributeName, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxHealth());
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp<float>(NewValue, 0.0f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	SetEffectProperties(Data, EffectProperties);
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	// Source = causer of the effect, Target = target of the effect (owner of this AS)
	const FGameplayEffectSpec& EffectSpec = Data.EffectSpec;
	//const UGameplayEffect* GameplayEffect = EffectSpec.Def.Get();
	const FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();

	Props.EffectContextHandle = EffectContextHandle;

	///Source:
	UAbilitySystemComponent* SourceASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	SetEffectEntityProperties(SourceASC, Props.Source);

	///Target:
	UAbilitySystemComponent* TargetASC = &Data.Target;
	SetEffectEntityProperties(TargetASC, Props.Target);

	//UE_LOG(LogTemp, Warning, TEXT("UAuraAttributeSet::SetEffectProperties"));
}

void UAuraAttributeSet::SetEffectEntityProperties(UAbilitySystemComponent* ASC, FEffectEntityProperties& EntityProps)
{
	if (IsValid(ASC))
	{
		EntityProps.AbilitySystemComponent = ASC;
		if (ASC->AbilityActorInfo.IsValid())
		{
			FGameplayAbilityActorInfo* AbilityActorInfo = ASC->AbilityActorInfo.Get();
			AActor* AvatarActor = AbilityActorInfo->AvatarActor.Get();
			AController* Controller = AbilityActorInfo->PlayerController.Get(); //PlayerController is set from OwnerActor by default
			if (Controller)
			{
				EntityProps.Controller = Controller;
			}
			if (AvatarActor)
			{
				EntityProps.AvatarActor = AvatarActor;
				if (ACharacter* Character = Cast<ACharacter>(AvatarActor))
				{
					EntityProps.Character = Character;
				}
				if (APawn* Pawn = Cast<APawn>(AvatarActor))
				{
					if (!Controller) //try to get Controller from AvatarActor
					{
						Controller = Pawn->GetController();
						EntityProps.Controller = Controller;
					}
				}
			}
		}
	}
}
