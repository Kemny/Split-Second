// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"
#include "../Weapons/Super_Gun.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "TimerManager.h"
#include "SplitSecondPlayerController.h"
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
  DashInputDelay = 0.3f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

  PlayerController = Cast<ASplitSecondPlayerController>(GetWorld()->GetFirstPlayerController());
  if (!ensure(PlayerController != nullptr)) { return; }
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
  if (!ensure(PlayerController != nullptr)) { return; }

  if (!PlayerController->bIsUsingGamepad)
  {
    RightKeyCount++;

    if (RightKeyCount >= 2)
    {
      FVector LaunchVelocity = GetActorRightVector() * DashMultiplier;

      LaunchCharacter(LaunchVelocity, true, true);
    }
  }
  else
  {
    FVector LaunchVelocity = GetActorRightVector() * DashMultiplier;

    LaunchCharacter(LaunchVelocity, true, true);
  }
}

void APlayerCharacter::DashRightReleased()
{
  GetWorldTimerManager().SetTimer(RightDashTimerHandle, this, &APlayerCharacter::ResetRightDash, DashInputDelay, false);
}

void APlayerCharacter::ResetRightDash()
{
  RightKeyCount = 0;
}

void APlayerCharacter::DashLeftPressed()
{
  if (!ensure(PlayerController != nullptr)) { return; }

  if (!PlayerController->bIsUsingGamepad)
  {
    LeftKeyCount++;

    if (LeftKeyCount >= 2)
    {
      FVector LaunchVelocity = GetActorRightVector() * DashMultiplier * -1;

      LaunchCharacter(LaunchVelocity, true, true);
    }
  }
  else
  {
    FVector LaunchVelocity = GetActorRightVector() * DashMultiplier * -1;

    LaunchCharacter(LaunchVelocity, true, true);
  }
}

void APlayerCharacter::ResetLeftDash()
{
  LeftKeyCount = 0;
}

void APlayerCharacter::DashLeftReleased()
{
  GetWorldTimerManager().SetTimer(LeftDashTimerHandle, this, &APlayerCharacter::ResetLeftDash, DashInputDelay, false);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
