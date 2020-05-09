// This project falls under CC-BY-SA lisence


#include "Super_Gun.h"
#include "Components/StaticMeshComponent.h"
#include "SplitSecondProjectile.h"
#include "../Player/PlayerCharacter.h"
#include "../AI/Super_AI_Character.h"
#include "Engine/World.h"
#include "../Player/SplitSecondPlayerController.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ASuper_Gun::ASuper_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
    RootComponent = GunMesh;

    FireRate = 0.5f;

    DefaultAmmoCount = 10;
    CurrentAmmoCount = DefaultAmmoCount;
    CurrentAmmoMax = DefaultAmmoCount;
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

void ASuper_Gun::FireGun()
{
  // try and fire a projectile
  if (ProjectileClass != NULL)
  {
    UWorld* const World = GetWorld();
    if (World != NULL)
    {
      if (CurrentAmmoCount > 0)
      {
        FRotator SpawnRotation;

        if (GetCurrentPawn()->IsA<APlayerCharacter>())
        {
          auto HitResult = World->GetFirstPlayerController<ASplitSecondPlayerController>()->LineTraceFromCamera(ECC_Camera);
          if (HitResult.GetActor())
          {
            SpawnRotation = UKismetMathLibrary::FindLookAtRotation(GunMesh->GetSocketLocation(FName("MuzzleLocation")), HitResult.Location);
          }
          else
          {
            SpawnRotation = CurrentPawn->GetControlRotation();
          }
        }
        else
        {
          SpawnRotation = CurrentPawn->GetControlRotation();
        }

        const FVector SpawnLocation = GunMesh->GetSocketLocation(FName("MuzzleLocation"));

        //Set Spawn Collision Handling Override
        FActorSpawnParameters ActorSpawnParams;
        ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

        // spawn the projectile at the muzzle
        World->SpawnActor<ASplitSecondProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

        if (GetCurrentPawn()->IsA<APlayerCharacter>())
        {
            CurrentAmmoCount--;
            UE_LOG(LogTemp, Log, TEXT("Current Ammo Count: %f"), CurrentAmmoCount)

            if (!ensure(GunMesh != nullptr)) { return; }
            GunMesh->CreateAndSetMaterialInstanceDynamic(1)->SetScalarParameterValue(TEXT("Emission Multiplier"), CurrentAmmoCount / CurrentAmmoMax);
        }

        LastTimeFired = GetWorld()->TimeSeconds;
      }
    }
  }
}

ASuper_Gun* ASuper_Gun::EquipGun(ACharacter* Character)
{
  if (!ensure(Character != nullptr)) { return nullptr; }

  APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(Character);
  ASuper_AI_Character* AIRef = Cast<ASuper_AI_Character>(Character);

  if (PlayerRef)
  {
    PlayerRef->CurrentGun = this;
    CurrentPawn = PlayerRef;

    this->AttachToComponent(PlayerRef->GunAttachMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
    GunMesh->bCastDynamicShadow = false;
    GunMesh->CastShadow = false;
    
    return this;
  }
  else if (AIRef)
  {
    AIRef->CurrentGun = this;
    CurrentPawn = AIRef;

    USceneComponent* EnemyScene = Cast<USceneComponent>(AIRef->GetMesh());
    if (!ensure(EnemyScene != nullptr)) { return nullptr; }

    this->AttachToComponent(EnemyScene, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GunSocket"));
    GunMesh->bCastDynamicShadow = false;
    GunMesh->CastShadow = false;

    return this;
  }
  else
  {
    return nullptr;
  }
}

ACharacter* ASuper_Gun::GetCurrentPawn()
{
  return CurrentPawn;
}
