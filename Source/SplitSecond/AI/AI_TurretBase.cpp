// This project falls under CC-BY-SA lisence


#include "AI_TurretBase.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Engine/World.h"
#include "../Health/HealthComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "../Weapons/Guns/AI/AIProjectile.h"
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
      // Shoot bullet out of right turret barrel
      const FRotator RightSpawnRotation = GetMesh()->GetSocketRotation(FName("Right Turret"));
      const FVector RightSpawnLocation = GetMesh()->GetSocketLocation(FName("Right Turret"));

      //Set Spawn Collision Handling Override
      FActorSpawnParameters ActorSpawnParams;
      ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	  // spawn the projectile at the muzzle
	  auto LocalCurrentProjectile = World->SpawnActor<ASplitSecondProjectile>(ProjectileClass, RightSpawnLocation, RightSpawnRotation, ActorSpawnParams);

	  auto LocalCurrentAIProjectile = Cast<AAIProjectile>(LocalCurrentProjectile);

	  if (LocalCurrentAIProjectile)
	  {
		  LocalCurrentAIProjectile->SetCurrentAI(this);
          LocalCurrentAIProjectile->ConstructEnemyProjectile();
	  }

      // Shoot bullet out of left turret barrel
      const FRotator LeftSpawnRotation = GetMesh()->GetSocketRotation(FName("Left Turret"));
      const FVector LeftSpawnLocation = GetMesh()->GetSocketLocation(FName("Left Turret"));

	  // spawn the projectile at the muzzle
	  LocalCurrentProjectile = World->SpawnActor<ASplitSecondProjectile>(ProjectileClass, LeftSpawnLocation, LeftSpawnRotation, ActorSpawnParams);

      LocalCurrentAIProjectile = Cast<AAIProjectile>(LocalCurrentProjectile);

	  if (LocalCurrentAIProjectile)
	  {
		  LocalCurrentAIProjectile->SetCurrentAI(this);
          LocalCurrentAIProjectile->ConstructEnemyProjectile();
	  }
    }
  }
}

void AAI_TurretBase::EnableRotationMovement(bool bEnable)
{
  RotatingMovement->SetActive(bEnable);

  bCurrentlyRotating = bEnable;
}

void AAI_TurretBase::OnTakeDamage()
{
	Highlight(EHighlightType::NONE);

	if (HealthComponent->GetHealth() <= 0)
	{
		bIsDead = true;
		SetActorEnableCollision(false);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		DetachFromControllerPendingDestroy();

        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, GetActorLocation(), GetActorRotation(), FVector(1), true, true, ENCPoolMethod::AutoRelease);

        Destroy();
	}
}

void AAI_TurretBase::BeginPlay()
{
  Super::BeginPlay();

  RotatingMovement->SetActive(false);
}

