// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "../Weapons/Super_Gun.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

#include "../SplitSecondGameState.h"
#include "PlayerMovementComponent.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	GunAttachMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunAttachMesh"));
	GunAttachMesh->SetupAttachment(FirstPersonCameraComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetWorld()->GetGameState<ASplitSecondGameState>();
	if (!ensure(GameState != nullptr)) { return; }
}
void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PlayerMovementComponent = Cast<UPlayerMovementComponent>(Super::GetMovementComponent());
}

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// Camera Movement
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void APlayerCharacter::OnFire()
{
  if (CurrentGun)
  {
    CurrentGun->FireGun();
  }
}

void APlayerCharacter::MoveForward(float Value)
{
	if (!ensure(GameState != nullptr)) { return; }

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value * GameState->GetGlobalTimeMultiplier());
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (!ensure(GameState != nullptr)) { return; }

	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value * GameState->GetGlobalTimeMultiplier());
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}
