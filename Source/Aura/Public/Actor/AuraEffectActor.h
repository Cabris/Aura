// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy:uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy:uint8
{
	RemoveOnOverlap,
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* EffectedTarget, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Rffects")
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Rffects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Rffects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Rffects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Rffects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Rffects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Rffects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Rffects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apply Rffects", meta = (ClampMin = "1", ClampMax = "10"))
	float ActorLevel = 1;

private:
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;

};
