// This project falls under CC-BY-SA lisence

#include "PlayerProjectile.h"
#include "Projectile_Explosion.h"
#include "GameFramework/DamageType.h"
#include "Kismet/GameplayStatics.h"
#include "../../BulletMovementComponent.h"
#include "Components/SphereComponent.h"
#include "../../../Player/PlayerCharacter.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

APlayerProjectile::APlayerProjectile()
{
	bShouldBounce = GetProjectileMovement()->bShouldBounce;
}

void APlayerProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor) return;
	if (OtherActor != this && !OtherActor->IsA<APlayerProjectile>())
	{
		// Called to apply damage to hit actor
		OnBulletHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

		if (bShouldBounce && CurrentBounce > BounceNum)
		{
			Destroy();
		}

		if (bIsExplosive)
		{
			FActorSpawnParameters SpawnParms;
			FTransform SpawnLocation = FTransform(GetActorRotation(), Hit.ImpactPoint, FVector(1));

			auto CurrentExplosion = GetWorld()->SpawnActor<AProjectile_Explosion>(ExplosionToSpawn, SpawnLocation, SpawnParms);
			CurrentExplosion->ApplyExplosionDamage(DamageValue, ExplosionUpTime);

			if (!ensure(Explosion != nullptr)) { return; }
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), Explosion, GetActorLocation());
			Destroy();
		}

		if (!OtherActor->IsA<APlayerCharacter>())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, Hit.TraceStart, GetActorRotation(), FVector(1), true, true, ENCPoolMethod::AutoRelease);
		}

		if (!bShouldBounce && !bIsExplosive)
		{
			Destroy();
		}
	}
}

void APlayerProjectile::OnBulletHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (bShouldBounce)
	{
		CalcReflection(Hit);
		++CurrentBounce;

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (!ensure(PlayerController != nullptr)) { return; }

		UGameplayStatics::ApplyDamage(OtherActor, DamageValue, PlayerController, this, UDamageType::StaticClass());

		if (!ensure(Bounce != nullptr)) { return; }
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), Bounce, GetActorLocation());
	}
	else
	{
		Super::OnBulletHit_Implementation(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);
	}
}