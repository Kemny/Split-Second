// This project falls under CC-BY-SA lisence

#include "SplitSecondPlayerController.h"
#include "SplitSecondHUD.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../Weapons/SplitSecondProjectile.h"
#include "../Weapons/Super_Gun.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "../AI/Super_AI_Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"
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

	Hud = GetHUD<ASplitSecondHUD>();
	if (!ensure(Hud != nullptr)) { return; }
	///TODO Remove Debug Menu
	Hud->ToggleDebugMenu();
}

void ASplitSecondPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!ensure(InputComponent != nullptr)) { return; }

	InputComponent->BindAction(FName("DEBUG_ShowDebugMenu"), EInputEvent::IE_Pressed, this, &ASplitSecondPlayerController::ShowDebugMenu);
	InputComponent->BindAxis(FName("DEBUG_IncreaseSlow"), this, &ASplitSecondPlayerController::IncreaseSlow);

	InputComponent->BindAction(FName("Ability_SlowTarget"), EInputEvent::IE_Pressed, this, &ASplitSecondPlayerController::SlowTarget);
}
void ASplitSecondPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TraceForActorsToSlow();
}

void ASplitSecondPlayerController::TraceForActorsToSlow()
{
	if (!ensure(Hud != nullptr)) { return; }

	auto HitResult = LineTraceFromCamera(ECollisionChannel::ECC_Visibility);

	///If line trace found an actor
	if (HitResult.GetActor())
	{
		///UnHighlight old NPC
		if(HoveredEnemy && HitResult.GetActor() != HoveredEnemy)
		{
			if (!HoveredEnemy->GetIsSlowed())
			{
				HoveredEnemy->GetMesh()->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor::Red);
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
					EnemyMesh->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0, 1, 1, 1));
				}
			}
		}
	}
}

FHitResult ASplitSecondPlayerController::LineTraceFromCamera(ECollisionChannel Collision)
{
	FHitResult HitResult;
	auto CameraLocation = PlayerCameraManager->GetCameraLocation();

	GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, (UKismetMathLibrary::Conv_RotatorToVector(PlayerCameraManager->GetCameraRotation()) * 50000) + CameraLocation, ECollisionChannel::ECC_Camera);
	return HitResult;
}
FHitResult ASplitSecondPlayerController::LineTraceFromCamera(ECollisionChannel Collision, FVector EndOffset)
{
	FHitResult HitResult;
	auto CameraLocation = PlayerCameraManager->GetCameraLocation();

	GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, (UKismetMathLibrary::Conv_RotatorToVector(PlayerCameraManager->GetCameraRotation()) * 50000) + CameraLocation + EndOffset, ECollisionChannel::ECC_Camera);

	return HitResult;
}
void ASplitSecondPlayerController::ShowDebugMenu()
{
	if (!ensure(Hud != nullptr)) { return; }
	Hud->ToggleDebugMenu();
}
void ASplitSecondPlayerController::IncreaseSlow(float Value)
{
	GetWorldSettings()->SetTimeDilation(UGameplayStatics::GetGlobalTimeDilation(GetWorld()) + Value *0.01);
}

void ASplitSecondPlayerController::SlowTarget()
{
	if (HoveredEnemy)
	{
		HoveredEnemy->GetSlowed(ActorSlowDuration, ActorSlowValue);
	}
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