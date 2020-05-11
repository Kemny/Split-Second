// This project falls under CC-BY-SA lisence


#include "Super_Gun.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/MeshComponent.h"
#include "../Player/PlayerCharacter.h"
#include "../AI/Super_AI_Character.h"
#include "../Player/SplitSecondPlayerController.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ASuper_Gun::ASuper_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    auto Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
    GunMesh->SetupAttachment(GetRootComponent());

    FireRate = 0.5f;

    DefaultAmmoCount = 10;
    CurrentAmmoCount = DefaultAmmoCount;
    CurrentAmmoMax = DefaultAmmoCount;

    GunMesh->bCastDynamicShadow = false;
    GunMesh->CastShadow = false;
}

void ASuper_Gun::OnInputPressed_Implementation()
{
  float FirstDelay = FMath::Max(LastTimeFired + FireRate - GetWorld()->TimeSeconds, 0.0f);

  GetWorldTimerManager().SetTimer(FireRateTimer, this, &ASuper_Gun::FireGun, FireRate, true, FirstDelay);
}

void ASuper_Gun::OnInputReleased_Implementation()
{
  GetWorldTimerManager().ClearTimer(FireRateTimer);
}

ACharacter* ASuper_Gun::GetCurrentPawn() const
{
  return CurrentPawn;
}

void ASuper_Gun::AfterPlayerFireGun(UMeshComponent* GunMeshToEdit)
{
    if (!ensure(GunMeshToEdit != nullptr)) { return; }

    CurrentAmmoCount--;
    UE_LOG(LogTemp, Log, TEXT("Current Ammo Count: %f"), CurrentAmmoCount);

    auto AmmoPercentage = CurrentAmmoCount / CurrentAmmoMax;
    GunMeshToEdit->CreateAndSetMaterialInstanceDynamic(1)->SetScalarParameterValue(TEXT("Emission Multiplier"), AmmoPercentage);
    GunMeshToEdit->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(TEXT("Color"), FLinearColor(AmmoPercentage, 0, 0, 1));

    LastTimeFired = GetWorld()->TimeSeconds;
}