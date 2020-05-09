// This project falls under CC-BY-SA lisence


#include "AI_TurretBase.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Weapons/SplitSecondProjectile.h"

AAI_TurretBase::AAI_TurretBase()
{
  RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));
  RotatingMovement->RotationRate = FRotator(0, 20, 0);

  ProjectileClass = ASplitSecondProjectile::StaticClass();
}

void AAI_TurretBase::ShootTurret()
{
  if (ProjectileClass != NULL)
  {
    UWorld* const World = GetWorld();
    if (World != NULL)
    {
      const FRotator SpawnRotation = GetControlRotation();
      const FVector SpawnLocation = GetMesh()->GetSocketLocation(FName("TraceSocket"));

      //Set Spawn Collision Handling Override
      FActorSpawnParameters ActorSpawnParams;
      ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

      // spawn the projectile at the muzzle
      World->SpawnActor<ASplitSecondProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
    }
  }
}

void AAI_TurretBase::EnableRotationMovement(bool bEnable)
{
  RotatingMovement->SetActive(bEnable);

  bCurrentlyRotating = bEnable;
}

void AAI_TurretBase::BeginPlay()
{
  Super::BeginPlay();

  RotatingMovement->SetActive(false);
}
