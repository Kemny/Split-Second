// This project falls under CC-BY-SA lisence


#include "Super_Gun.h"
#include "Components/StaticMeshComponent.h"
#include "SplitSecondProjectile.h"
#include "../Player/PlayerCharacter.h"
#include "Engine/World.h"

// Sets default values
ASuper_Gun::ASuper_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
  RootComponent = GunMesh;

  FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
  FP_MuzzleLocation->SetupAttachment(RootComponent, "MuzzleLocation");
}

void ASuper_Gun::FireGun()
{
  // try and fire a projectile
  if (ProjectileClass != NULL)
  {
    UWorld* const World = GetWorld();
    if (World != NULL)
    {
      const FRotator SpawnRotation = CurrentPawn->GetControlRotation();
      // MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
      if (!ensure(FP_MuzzleLocation != nullptr)) { return; }
      const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();

      //Set Spawn Collision Handling Override
      FActorSpawnParameters ActorSpawnParams;
      ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

      // spawn the projectile at the muzzle
      World->SpawnActor<ASplitSecondProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
    }
  }
}

ASuper_Gun* ASuper_Gun::EquipGun(ACharacter* Character)
{
  if (!ensure(Character != nullptr)) { return nullptr; }

  APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(Character);

  if (PlayerRef)
  {
    PlayerRef->CurrentGun = this;
    CurrentPawn = PlayerRef;

    this->AttachToComponent(PlayerRef->GunAttachMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
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
