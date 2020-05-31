// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "PlayerProjectile.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../../AI/Super_AI_Character.h"
#include "../../../AI/TurretShield.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/DamageType.h"
#include "Sound/SoundAttenuation.h"
#include "../../../SplitSecondGameMode.h"

APlayerProjectile::APlayerProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ExplosionRadiusCollision = CreateDefaultSubobject<USphereComponent>("Explosion Radius");
	ExplosionRadiusCollision->SetupAttachment(RootComponent);
	ExplosionRadiusCollision->SetSphereRadius(ExplosionRadius);

	ExplosionRadiusCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ExplosionRadiusCollision->SetCollisionResponseToAllChannels(ECR_Overlap);

	//If it should bounce was set in blueprint for movement component, this will replicate it
	bShouldBounce = GetProjectileMovement()->bShouldBounce;
}

void APlayerProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor || OtherActor == this) return;

	bool bShouldDestroy = true;

	// Called to apply damage to hit actor
	UGameplayStatics::ApplyDamage(OtherActor, Damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, UDamageType::StaticClass());
	
	if (!ensure(BulletImpactSound != nullptr)) { return; }
	USoundBase* SoundToPlay = BulletImpactSound;
	UNiagaraSystem* SytemToSpawn = DefaultCollisionParticle;

	SetExplosive(SytemToSpawn, SoundToPlay);

	SetBounce(OtherActor, Hit, SoundToPlay, bShouldDestroy);
	
	SetPiercing(OtherActor, SoundToPlay, bShouldDestroy);

	PlayShieldSound(OtherActor, SoundToPlay);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SytemToSpawn, GetActorLocation(), FRotator(0), FVector(1), true, true, ENCPoolMethod::AutoRelease);

	if (SoundToPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation());
	}

	if (bShouldDestroy)
	{
		Destroy();
	}
}

void APlayerProjectile::OnBulletOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	bool bShouldDestroy = true;

	// Called to apply damage to hit actor
	UGameplayStatics::ApplyDamage(OtherActor, Damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, UDamageType::StaticClass());

	if (!ensure(BulletImpactSound != nullptr)) { return; }
	USoundBase* SoundToPlay = BulletImpactSound;
	UNiagaraSystem* SytemToSpawn = DefaultCollisionParticle;

	SetExplosive(SytemToSpawn, SoundToPlay);

	SetPiercing(OtherActor, SoundToPlay, bShouldDestroy);

	PlayShieldSound(OtherActor, SoundToPlay);

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SytemToSpawn, GetActorLocation(), FRotator(0), FVector(1), true, true, ENCPoolMethod::AutoRelease);

	if (SoundToPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay, GetActorLocation());
	}

	if (bShouldDestroy)
	{
		Destroy();
	}
}

FVector APlayerProjectile::CalculateReflectionVelocity(FVector ReflectorNormal)
{
	auto MyVelocity = GetProjectileMovement()->Velocity;
	return BounceSpeedLoss * (-2 * FVector::DotProduct(MyVelocity, ReflectorNormal) * ReflectorNormal + MyVelocity);
}
void APlayerProjectile::ReflectProjectile(const FHitResult& Hit)
{
	FVector ReflectedVelocity = CalculateReflectionVelocity(Hit.Normal);

	GetProjectileMovement()->bShouldBounce = true;
	GetProjectileMovement()->SetVelocityInLocalSpace(ReflectedVelocity);
	ReflectedVelocity.Normalize();
	SetActorRotation(ReflectedVelocity.Rotation());
}

void APlayerProjectile::ApplyExplosionDamage()
{
	TArray<AActor*> ActorsToDamage;
	ExplosionRadiusCollision->GetOverlappingActors(ActorsToDamage, ASuper_AI_Character::StaticClass());

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	for (auto ActorToDamage : ActorsToDamage)
	{
		if (ActorToDamage)
		{
			UGameplayStatics::ApplyDamage(ActorToDamage, Damage, PlayerController, this, UDamageType::StaticClass());
		}
	}
}

void APlayerProjectile::SetExplosive(UNiagaraSystem*& SytemToSpawn, USoundBase*& SoundToPlay)
{
	if (bIsExplosive)
	{
		SytemToSpawn = ExplodingBulletFX;
		ApplyExplosionDamage();

		if (!ensure(Explosion != nullptr)) { return; }
		SoundToPlay = Explosion;
	}
}

void APlayerProjectile::SetBounce(AActor* OtherActor, const FHitResult& Hit, USoundBase*& SoundToPlay, bool& bShouldDestroy)
{
	if (bShouldBounce && CurrentBounce < BounceNum)
	{
		ReflectProjectile(Hit);
		++CurrentBounce;

		if (!ensure(Bounce != nullptr)) { return; }
		SoundToPlay = Bounce;

		bShouldDestroy = false;
	}
	else
	{
		bShouldDestroy = true;
	}
}

void APlayerProjectile::PlayShieldSound(AActor* OtherActor, USoundBase*& SoundToPlay)
{
	if (OtherActor->IsA<ATurretShield>())
	{
		SoundToPlay = ShieldImpact;
	}
}

void APlayerProjectile::SetPiercing(AActor* OtherActor, USoundBase*& SoundToPlay, bool& bShouldDestroy)
{
	if (bIsPiercing && OtherActor->IsA<ASuper_AI_Character>())
	{
		if (!ensure(Piercing != nullptr)) { return; }
		SoundToPlay = Piercing;

		bShouldDestroy = false;
	}
}
