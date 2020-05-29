// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007


#include "AI_TurretBase.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Engine/World.h"
#include "../Health/HealthComponent.h"
#include "../Weapons/Guns/AI/AIProjectile.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Weapons/SplitSecondProjectile.h"
#include "Kismet/GameplayStatics.h"

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
		// Shoot bullet out of right turret barrel
		const FRotator RightSpawnRotation = GetMesh()->GetSocketRotation(FName("Right Turret"));
		const FVector RightSpawnLocation = GetMesh()->GetSocketLocation(FName("Right Turret"));
		const FTransform RightSpawnTransform = FTransform(RightSpawnRotation, RightSpawnLocation);

		SpawnTurretProjectile(RightSpawnTransform);

		// Shoot bullet out of left turret barrel
		const FRotator LeftSpawnRotation = GetMesh()->GetSocketRotation(FName("Left Turret"));
		const FVector LeftSpawnLocation = GetMesh()->GetSocketLocation(FName("Left Turret"));
		const FTransform LeftSpawnTransform = FTransform(LeftSpawnRotation, LeftSpawnLocation);

		SpawnTurretProjectile(LeftSpawnTransform);

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, GetActorLocation());
	}
}
void AAI_TurretBase::SpawnTurretProjectile(FTransform SpawnTransform)
{
	// spawn the projectile at the muzzle
	if (auto LocalCurrentProjectile = GetWorld()->SpawnActorDeferred<ASplitSecondProjectile>(ProjectileClass, SpawnTransform))
	{
		if (auto LocalCurrentAIProjectile = Cast<AAIProjectile>(LocalCurrentProjectile))
		{
			LocalCurrentAIProjectile->SetDamage(Damage);
			LocalCurrentAIProjectile->FinishSpawning(SpawnTransform);
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

