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
#include "../SplitSecondPlayerState.h"
#include "../Health/HealthComponent.h"
#include "../UI/PlayerUI.h"
#include "UObject/ConstructorHelpers.h"
#include "../UI/PopupMessage.h"

APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    ConstructorHelpers::FClassFinder<UPlayerUI> BP_PlayerUIClass(TEXT("/Game/Blueprint/UI/WBP_PlayerUI"));
    if (BP_PlayerUIClass.Class) PlayerUIClass = BP_PlayerUIClass.Class;

    ConstructorHelpers::FClassFinder<UPopupMessage> BP_PopupMessageClass(TEXT("/Game/Blueprint/UI/WBP_PopupMessage"));
    if (BP_PopupMessageClass.Class) PopupMessageClass = BP_PopupMessageClass.Class;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	GunAttachMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunAttachMesh"));
	GunAttachMesh->SetupAttachment(FirstPersonCameraComponent);

    HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
    HealthComponent->OnHealthChanged.BindUFunction(this, FName("OnTakeDamage"));

    BossTeleportPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Boss Teleport Point"));
    BossTeleportPoint->SetupAttachment(RootComponent);

    DashMultiplier = 400.f;
    DashInputDelay = 0.3f;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

    PlayerController = Cast<ASplitSecondPlayerController>(GetWorld()->GetFirstPlayerController());
    if (!ensure(PlayerController != nullptr)) { return; }
    PlayerState = GetPlayerState<ASplitSecondPlayerState>();
    if (!ensure(PlayerState != nullptr)) { return; }


    PlayerUI = CreateWidget<UPlayerUI>(GetWorld(), PlayerUIClass);
    if (!ensure(PlayerUI != nullptr)) { return; }
    PlayerUI->AddToPlayerScreen();
    PlayerUI->UpdateHealth(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
}

void APlayerCharacter::EquipGun(TSubclassOf<class ASuper_Gun> GunClass)
{
    if (GunClass)
    {
        CurrentGun = GetWorld()->SpawnActor<ASuper_Gun>(GunClass);
        if (!ensure(CurrentGun != nullptr)) { return; }

        CurrentGun->SetCurrentPawn(this);
        CurrentGun->AttachToComponent(GunAttachMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
    }
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
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::OnFirePressed);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &APlayerCharacter::OnFireReleased);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	// Camera Movement
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

    // Dashing
	PlayerInputComponent->BindAction("Dash Toggle", IE_Pressed, this, &APlayerCharacter::AllowDash);
	PlayerInputComponent->BindAction("Dash Toggle", IE_Released, this, &APlayerCharacter::ForbidDash);
}

void APlayerCharacter::Jump()
{
    Super::Jump();

    if (!ensure(PlayerState != nullptr)) { return; }

    if (CurrentJump < PlayerState->CurrentStats.MaxJumps && CurrentJump > 0)
    {
        LaunchCharacter(FVector(0, 0, 600), false, true);
    }
    ++CurrentJump;
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
    Super::Landed(Hit);
    CurrentJump = 0;
}

void APlayerCharacter::OnFirePressed()
{
  if (CurrentGun)
  {
    CurrentGun->OnInputPressed();
  }
}

void APlayerCharacter::OnFireReleased()
{
  if (CurrentGun)
  {
    CurrentGun->OnInputReleased();
  }
}

void APlayerCharacter::MoveForward(float Value)
{
    if (bCanDash && bDashOffCooldown && Value != 0)
    {
        LaunchCharacter(GetActorForwardVector() * Value * DashMultiplier, true, true);
        bDashOffCooldown = false;
        ForbidDash();
        GetWorldTimerManager().SetTimer(DashTimerHandle, this, &APlayerCharacter::ResetDash, DashInputDelay, false);
    } 
    else if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
    if (bCanDash && bDashOffCooldown && Value != 0)
    {
        LaunchCharacter(GetActorRightVector() * Value * DashMultiplier, true, true);
        bDashOffCooldown = false;
        ForbidDash();
        GetWorldTimerManager().SetTimer(DashTimerHandle, this, &APlayerCharacter::ResetDash, DashInputDelay, false);
    }
    else if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}


void APlayerCharacter::ResetDash()
{
    bDashOffCooldown = true;
}

void APlayerCharacter::OnTakeDamage()
{
    if (!ensure(PlayerUI != nullptr)) { return; }
    if (!ensure(PlayerState != nullptr)) { return; }

    PlayerUI->UpdateHealth(HealthComponent->GetHealth(), HealthComponent->GetMaxHealth());
    if (HealthComponent->GetHealth() <= 0 && !bIsInvinclible)
    {
        if (PlayerState->CurrentStats.bHasExtraLife)
        {
            HealthComponent->Heal(HealthComponent->GetMaxHealth());
            PlayerState->CurrentStats.bHasExtraLife = false;
        }
        else
        {
            if (!ensure(PlayerController != nullptr)) { return; }

            PlayerUI->RemoveFromParent();
            PlayerController->HandlePlayerDeath();

            if (auto Message = CreateWidget<UPopupMessage>(GetWorld(), PopupMessageClass))
            {
                Message->ShowPopupMessage(FKey("F"), FText::FromString("YOU DIED \n PRESS F TO RESTART"));
                Message->OnConditionFufilled.BindUFunction(this, TEXT("OnConfirmedDeath"));
            }
        }
    }
}

void APlayerCharacter::OnConfirmedDeath()
{
    if (!ensure(PlayerController != nullptr)) { return; }
    PlayerController->HandlePlayerConfirmedDeath();
}