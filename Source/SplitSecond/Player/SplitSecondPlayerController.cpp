// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "SplitSecondPlayerController.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../Weapons/SplitSecondProjectile.h"
#include "../Weapons/Super_Gun.h"
#include "../AI/Super_AI_Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../SplitSecondPlayerState.h"
#include "PlayerCharacter.h"
#include "../UI/PlayerUI.h"
#include "../SplitSecondPlayerState.h"

ASplitSecondPlayerController::ASplitSecondPlayerController()
{
  bIsUsingGamepad = false;
  bResetGamepadDetectionAfterNoInput = true;
  GamepadTimeout = 5.f;
}

void ASplitSecondPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = GetPawn<APlayerCharacter>();
	if (!ensure(PlayerCharacter != nullptr)) { return; }
	PlayerCharacter->GetFirstPersonCameraComponent()->PostProcessSettings = NormalPostProcess;
}

void ASplitSecondPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!ensure(InputComponent != nullptr)) { return; }

	InputComponent->BindAction(FName("Ability_SlowTarget"), EInputEvent::IE_Pressed, this, &ASplitSecondPlayerController::SlowTarget);
	InputComponent->BindAction(FName("Ability_SlowGame"), EInputEvent::IE_Pressed, this, &ASplitSecondPlayerController::SlowGame);
}
void ASplitSecondPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForActorsToSlow();
}

void ASplitSecondPlayerController::TraceForActorsToSlow()
{
	auto HitResult = LineTraceFromCamera(ECollisionChannel::ECC_Visibility);

	///If line trace found an actor
	if (HitResult.GetActor())
	{
		///UnHighlight old NPC
		if(HoveredEnemy && HitResult.GetActor() != HoveredEnemy)
		{
			if (!HoveredEnemy->GetIsSlowed())
			{
				HoveredEnemy->Highlight(EHighlightType::NONE);
				HoveredEnemy = nullptr;
			}
		}

		else if (HitResult.GetActor()->IsA<ASuper_AI_Character>())
		{
			///Set Hovered Enemy
			HoveredEnemy = Cast<ASuper_AI_Character>(HitResult.GetActor());
			///Hightlight new Enemy
			if (auto EnemyMesh = HoveredEnemy->GetMesh())
			{
				if (!HoveredEnemy->GetIsSlowed())
				{
					HoveredEnemy->OnDestroyed.AddUniqueDynamic(this, &ASplitSecondPlayerController::OnSlowingTargetDeath);
					HoveredEnemy->Highlight(EHighlightType::Highlight);
				}
			}
		}
	}
}
void ASplitSecondPlayerController::OnSlowingTargetDeath(AActor* KilledAI)
{
	if (HoveredEnemy == KilledAI)
	{
		HoveredEnemy = nullptr;
	}
}
void ASplitSecondPlayerController::SlowTarget()
{
	auto SplitSecondPlayerState = GetPawn()->GetPlayerState<ASplitSecondPlayerState>();
	if (!ensure(PlayerState != nullptr)) { return; }
	if (HoveredEnemy && !HoveredEnemy->GetIsSlowed())
	{
		HoveredEnemy->GetSlowed(SplitSecondPlayerState->CurrentStats.ActorSlowDuration, SplitSecondPlayerState->CurrentStats.ActorSlowValue);

		if (!ensure(EnemySlowSound != nullptr)) { return; }
		UGameplayStatics::PlaySound2D(GetWorld(), EnemySlowSound);
	}
}
void ASplitSecondPlayerController::SlowGame()
{
	if (GameSlowOnCooldown) return;

	GameSlowOnCooldown = true;

	auto SplitSecondPlayerState = GetPawn()->GetPlayerState<ASplitSecondPlayerState>();
	if (!ensure(SplitSecondPlayerState != nullptr)) { return; }
	if (!ensure(PlayerCharacter != nullptr)) { return; }
	PlayerCharacter->GetPlayerUI()->ActivateTimeSlow(SplitSecondPlayerState->CurrentStats.GameSlowCooldown, SplitSecondPlayerState->CurrentStats.GameSlowDuration);

	FTimerHandle Handle;
	FTimerHandle Handle2;
	GetWorldTimerManager().SetTimer(Handle, this, &ASplitSecondPlayerController::OnGameSlowCooldownFinished, SplitSecondPlayerState->CurrentStats.GameSlowCooldown, false);
	GetWorldTimerManager().SetTimer(Handle2, this, &ASplitSecondPlayerController::OnGameSlowFinished, SplitSecondPlayerState->CurrentStats.GameSlowDuration, false);

	OnPlayerSlowGame.ExecuteIfBound();

	PlayerCharacter->GetFirstPersonCameraComponent()->PostProcessSettings = SlowedPostProcess;

	if (!ensure(GameSlowSound != nullptr)) { return; }
	UGameplayStatics::PlaySound2D(GetWorld(), GameSlowSound);
}
void ASplitSecondPlayerController::OnGameSlowCooldownFinished()
{
	GameSlowOnCooldown = false;
}
void ASplitSecondPlayerController::OnGameSlowFinished()
{
	if (!ensure(PlayerCharacter != nullptr)) { return; }
	PlayerCharacter->GetFirstPersonCameraComponent()->PostProcessSettings = NormalPostProcess;
}

FHitResult ASplitSecondPlayerController::LineTraceFromCamera(ECollisionChannel Collision, FVector EndOffset)
{
	FHitResult HitResult;
	auto CameraLocation = PlayerCameraManager->GetCameraLocation();

	GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, (UKismetMathLibrary::Conv_RotatorToVector(PlayerCameraManager->GetCameraRotation()) * 50000) + CameraLocation + EndOffset, ECollisionChannel::ECC_Camera);

	return HitResult;
}

bool ASplitSecondPlayerController::InputAxis(FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
  bool ret = Super::InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
  _UpdateGamepad(bGamepad);
  return ret;
}

bool ASplitSecondPlayerController::InputKey(FKey Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
  bool ret = Super::InputKey(Key, EventType, AmountDepressed, bGamepad);
  _UpdateGamepad(bGamepad);
  return ret;
}

void ASplitSecondPlayerController::PlayerTick(float DeltaTime)
{
  Super::PlayerTick(DeltaTime);

  if (IsLocalController() && bResetGamepadDetectionAfterNoInput && bIsUsingGamepad)
  {
    float now = GetWorld()->TimeSeconds;
    if (now > LastGamepadInputTime + GamepadTimeout)
    {
      bIsUsingGamepad = false;
      ControllerDisconnected();
    }
    else
    {
      bIsUsingGamepad = true;
    }
  }
}

void ASplitSecondPlayerController::HandlePlayerDeath()
{
	GetWorldSettings()->TimeDilation = 0.01;
	OnPlayerDeath.ExecuteIfBound();
}
void ASplitSecondPlayerController::HandlePlayerConfirmedDeath()
{
	OnPlayerConfirmedDeath.ExecuteIfBound();
}