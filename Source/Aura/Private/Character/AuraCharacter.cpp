// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"

AAuraCharacter::AAuraCharacter()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, DEFAULT_AURA_ROTATE_RATE_YAW, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//called on the server
	InitAbilityActorInfo();
	UE_LOG(LogTemp, Log, TEXT("AAuraCharacter::InitAbilityActorInfo called on the server"));
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//called on the client
	InitAbilityActorInfo();
	UE_LOG(LogTemp, Log, TEXT("AAuraCharacter::InitAbilityActorInfo called on the client"));
}

void AAuraCharacter::PitchCamera(float AxisValue)
{
	FRotator Rotator = FRotator(AxisValue, 0, 0);
	SpringArm->AddRelativeRotation(Rotator);
}

void AAuraCharacter::YawCamera(float AxisValue)
{
	FRotator Rotator = FRotator(0, AxisValue, 0);
	SpringArm->AddRelativeRotation(Rotator);
}

void AAuraCharacter::InitAbilityActorInfo()
{
	//AAuraPlayerState* state = CastChecked<AAuraPlayerState>(GetPlayerState());
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(AuraPlayerState, this);
	AttributeSet = AuraPlayerState->GetAttributeSet();
}
