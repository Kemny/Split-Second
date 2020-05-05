// This project falls under CC-BY-SA lisence

#include "SplitSecondPlayerController.h"
#include "SplitSecondHUD.h"
#include "Engine/World.h"
#include "GameFramework/WorldSettings.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../Weapons/SplitSecondProjectile.h"
#include "Materials/MaterialInstanceDynamic.h"

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

bool ASplitSecondPlayerController::TraceForActorsToSlow()
{
	if (!ensure(Hud != nullptr)) { return false; }

	FVector ScreenWorldLocation;
	FVector ScreenWorldDirection;
	DeprojectScreenPositionToWorld(Hud->GetCrosshairPosition().X, Hud->GetCrosshairPosition().Y, ScreenWorldLocation, ScreenWorldDirection);

	FHitResult HitResult;
	///If line trace found an actor
	if (GetWorld()->LineTraceSingleByChannel(HitResult, ScreenWorldLocation, ScreenWorldDirection * 10000, ECollisionChannel::ECC_Visibility))
	{
		///UnHighlight any old projectiles
		if (HoveredProjectile && HitResult.GetActor() != HoveredProjectile)
		{
			if (auto BulletMesh = HoveredProjectile->GetBulletMesh())
			{
				BulletMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor::Red);
				BulletMesh->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor::Red);
				HoveredProjectile = nullptr;
			}
		}
		///Check New Actor
		if (HitResult.GetActor()->IsA<ASplitSecondProjectile>())
		{
			///Set Hovered Bullet
			HoveredProjectile = Cast<ASplitSecondProjectile>(HitResult.GetActor());
			///Hightlight new Bullet
			if (auto BulletMesh = HoveredProjectile->GetBulletMesh())
			{
				BulletMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0, 1, 1, 1));
				BulletMesh->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0, 1, 1, 1));
			}
		}
	}
	return true;
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
	if (HoveredProjectile)
	{
		HoveredProjectile->GetSlowed(ActorSlowDuration, ActorSlowValue);
	}
}