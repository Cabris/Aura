// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(rootComponent);
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* EffectedTarget, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(EffectedTarget);
	//if (ASCInterface)
	//{
	//auto ASC=ASCInterface->GetAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(EffectedTarget);
	if (TargetASC)
	{
		check(GameplayEffectClass);
		FGameplayEffectContextHandle EffetContextHandle = TargetASC->MakeEffectContext();
		EffetContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, ActorLevel, EffetContextHandle);
		const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		const bool bIsInfinite = EffectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
		if (bIsInfinite && InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			ActiveEffectHandles.Add(ActiveEffectHandle, TargetASC);
		}

		//Survey result: obj == obj2
		//obj: name: /Game/Blueprints/Actor/Crystal/GE_CrystalMana.Default__GE_CrystalMana_C,p:00000B5ABFC9F000;
		//obj2: name: /Game/Blueprints/Actor/Crystal/GE_CrystalMana.Default__GE_CrystalMana_C, 00000B5ABFC9F000
		/*const UGameplayEffect* obj = GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
		const UGameplayEffect* obj2 = EffectSpecHandle.Data.Get()->Def.Get();
		check(obj);
		check(obj2);
		bool HasDuration = EffectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::HasDuration;
		UE_LOG(LogTemp, Warning, TEXT("obj: name: %s,p:%p; obj2: name: %s, %p"), *obj->GetPathName(), obj, *obj2->GetPathName(), obj2);
		UE_LOG(LogTemp, Warning, TEXT("HasDuration: %d"), HasDuration);*/
		//
	}
	//}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}

	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}

	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}

	if (InfiniteEffectRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (TPair<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePair : ActiveEffectHandles)
		{
			if (HandlePair.Value == TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key, 1);
				HandlesToRemove.Add(HandlePair.Key);
			}
		}
		for (FActiveGameplayEffectHandle Handle : HandlesToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}
