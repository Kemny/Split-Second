// This project falls under CC-BY-SA lisence

#include "AIProjectile.h"
#include "../../../Player/PlayerCharacter.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

void AAIProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor) return;
	if (OtherActor != this && !OtherActor->IsA<AAIProjectile>())
	{
		// Called to apply damage to hit actor
		OnBulletHit(HitComp, OtherActor, OtherComp, NormalImpulse, Hit);

		if (!OtherActor->IsA<APlayerCharacter>())
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, Hit.TraceStart, GetActorRotation(), FVector(1), true, true, ENCPoolMethod::AutoRelease);
		}

		Destroy();
	}
}