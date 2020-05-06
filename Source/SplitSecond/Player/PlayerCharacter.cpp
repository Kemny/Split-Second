// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "../Weapons/Super_Gun.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"

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

  DashMultiplier = 400.f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

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

  PlayerInputComponent->BindAction("Dash Right", IE_Pressed, this, &APlayerCharacter::DashRightPressed);
  PlayerInputComponent->BindAction("Dash Right", IE_Released, this, &APlayerCharacter::DashRightReleased);

  PlayerInputComponent->BindAction("Dash Left", IE_Pressed, this, &APlayerCharacter::DashLeftPressed);
  PlayerInputComponent->BindAction("Dash Left", IE_Released, this, &APlayerCharacter::DashLeftReleased);
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
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::DashRightPressed()
{
  RightKeyCount++;

  if (RightKeyCount >= 2)
  {
    FVector LaunchVelocity = GetActorRightVector() * DashMultiplier;

    LaunchCharacter(LaunchVelocity, false, false);
  }
}

void APlayerCharacter::DashRightReleased()
{
  GetWorldTimerManager().SetTimer(RightDashTimerHandle, this, &APlayerCharacter::ResetRightDash, 0.3f, false);
}

void APlayerCharacter::ResetRightDash()
{
  RightKeyCount = 0;
}

void APlayerCharacter::DashLeftPressed()
{
  LeftKeyCount++;

  if (LeftKeyCount >= 2)
  {
    FVector LaunchVelocity = GetActorRightVector() * DashMultiplier * -1;

    LaunchCharacter(LaunchVelocity, false, false);
  }
}

void APlayerCharacter::ResetLeftDash()
{
  LeftKeyCount = 0;
}

void APlayerCharacter::DashLeftReleased()
{
  GetWorldTimerManager().SetTimer(LeftDashTimerHandle, this, &APlayerCharacter::ResetLeftDash, 0.3f, false);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
