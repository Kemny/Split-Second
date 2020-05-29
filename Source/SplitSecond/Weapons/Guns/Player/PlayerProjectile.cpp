// This project falls under CC-BY-SA lisence

#include "PlayerProjectile.h"
#include "GameFramework/DamageType.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../../AI/Super_AI_Character.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/DamageType.h"

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

void APlayerProjectile::OnBulletOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this) return;

	bool bShouldDestroy = true;
	
	// Called to apply damage to hit actor
	UGameplayStatics::ApplyDamage(OtherActor, Damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, UDamageType::StaticClass());

	UNiagaraSystem* SytemToSpawn = DefaultCollisionParticle;
	USoundBase* SoundToPlay = nullptr;

	if (bIsExplosive)
	{
		SytemToSpawn = ExplodingBulletFX;
		ApplyExplosionDamage();

		if (!ensure(Explosion != nullptr)) { return; }
		SoundToPlay = Explosion;
	}

	if (bShouldBounce && CurrentBounce < BounceNum)
	{
		CalcReflection(SweepResult);
		++CurrentBounce;

		if (!ensure(Bounce != nullptr)) { return; }
		SoundToPlay = Bounce;

		bShouldDestroy = false;
	}

	if (bIsPiercing && OtherActor->IsA<ASuper_AI_Character>())
	{
		if (!ensure(Piercing != nullptr)) { return; }
		SoundToPlay = Piercing;

		bShouldDestroy = false;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SytemToSpawn, GetActorLocation(), GetActorRotation(), FVector(1), true, true, ENCPoolMethod::AutoRelease);

	if (SoundToPlay)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Explosion, GetActorLocation());
	}

	if (bShouldDestroy)
	{
		Destroy();
	}
}

void APlayerProjectile::CalcReflection(const FHitResult& Hit)
{
	GetProjectileMovement()->bShouldBounce = true;
	auto MyVelocity = GetProjectileMovement()->Velocity;

	FVector ReflectedVelocity = BounceSpeedLoss * (-2 * FVector::DotProduct(MyVelocity, Hit.Normal) * Hit.Normal + MyVelocity);
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