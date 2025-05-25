// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interfactions/CameraControllableInterface.h"
#include "AuraCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
/**
 *
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public ICameraControllableInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void PitchCamera(float AxisValue) override;
	virtual void YawCamera(float AxisValue) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> Camera;
	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;

private:
	void InitAbilityActorInfo();
};
