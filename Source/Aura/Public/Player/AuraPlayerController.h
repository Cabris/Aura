// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
//#include "EnhancedInputComponent.h"
#include "AuraPlayerController.generated.h"

class IEnemyInterface;
// Forward declarations
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MRBAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	float CameraRotationMultiplier = 100;

	TScriptInterface<IEnemyInterface> PreviousTarget;
	TScriptInterface<IEnemyInterface> CurrentTarget;
	FVector2D ViewportSize;
	bool IsDragCamera;
	void TraceCursor();
	void DragCamera(float DeltaTime);
	void Move(const FInputActionValue& inputActionValue);
	void MRB_Started(const FInputActionValue& inputActionValue);
	void MRB_Completed(const FInputActionValue& inputActionValue);
};
