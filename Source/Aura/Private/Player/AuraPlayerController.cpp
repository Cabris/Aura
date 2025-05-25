// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include <EnhancedInputComponent.h>
#include "InputActionValue.h"
#include "Aura/Aura.h"
#include "Interfactions/CameraControllableInterface.h"
#include "Interfactions/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; //for multiplayer usage
	IsDragCamera = false;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext); //ensure that the AuraContext is set in the editor

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	// Set the input mode to UI only
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);

	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	EnhancedInputComponent->BindAction(MRBAction, ETriggerEvent::Started, this, &AAuraPlayerController::MRB_Started);
	EnhancedInputComponent->BindAction(MRBAction, ETriggerEvent::Completed, this,
	                                   &AAuraPlayerController::MRB_Completed);
}

void AAuraPlayerController::TraceCursor()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (!Hit.bBlockingHit) return;
	PreviousTarget = CurrentTarget;
	CurrentTarget = Hit.GetActor();

	//UE_LOG(LogTemp, Warning, TEXT("case: 0"));
	if (PreviousTarget == nullptr && CurrentTarget != nullptr)
	{
		CurrentTarget->HightlightActor();
	}
	if (PreviousTarget != nullptr && CurrentTarget == nullptr)
	{
		PreviousTarget->UnHightlightActor();
	}
	if (PreviousTarget != nullptr && CurrentTarget != nullptr && PreviousTarget != CurrentTarget)
	{
		PreviousTarget->UnHightlightActor();
		CurrentTarget->HightlightActor();
	}
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	TraceCursor();
	DragCamera(DeltaTime);
}

void AAuraPlayerController::Move(const FInputActionValue& inputActionValue)
{
	FVector2D inputAxisVector = inputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, inputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, inputAxisVector.X);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No pawn controlled"));
	}
}

void AAuraPlayerController::DragCamera(float DeltaTime)
{
	if (!IsDragCamera) return;
	if (ICameraControllableInterface* CameraControllable = GetPawn<ICameraControllableInterface>())
	{
		float MouseDeltaX;
		float MouseDeltaY;
		GetInputMouseDelta(MouseDeltaX, MouseDeltaY);

		const float YawDelta = MouseDeltaX * CameraRotationMultiplier / ViewportSize.X;
		const float PitchDelta = MouseDeltaY * CameraRotationMultiplier / ViewportSize.Y;
		//UE_LOG(LogTemp, Log, TEXT("DragCamera::Delta: %f, %f"), YawDelta, PitchDelta);
		CameraControllable->YawCamera(YawDelta);
		CameraControllable->PitchCamera(PitchDelta);
		FRotator Rotation = GetControlRotation();
		Rotation.Yaw += YawDelta;
		Rotation.Pitch += PitchDelta;
		SetControlRotation(Rotation);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No pawn CameraControllable"));
	}
}

void AAuraPlayerController::MRB_Started(const FInputActionValue& inputActionValue)
{
	IsDragCamera = true;
	//base point: LeftTop
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
}

void AAuraPlayerController::MRB_Completed(const FInputActionValue& inputActionValue)
{
	IsDragCamera = false;
	/*float mouseX;
	float mouseY;
	GetMousePosition(mouseX, mouseY);
	UE_LOG(LogTemp, Log, TEXT("MRB_Completed::Mouse Location: %f, %f"), mouseX, mouseY);*/
}
