// This project falls under CC-BY-SA lisence

#include "AIProjectile.h"
#include "../../../Player/PlayerCharacter.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "../../../AI/Super_AI_Character.h"
#include "../../../SplitSecondGameMode.h"
#include "TimerManager.h"

void AAIProjectile::BeginPlay()
{
	Super::BeginPlay();

	auto Gamemode = GetWorld()->GetAuthGameMode<ASplitSecondGameMode>();
	if (!ensure(Gamemode != nullptr)) { return; }

	if (Gamemode->GetIsGameSlowed())
	{
		CustomTimeDilation = Gamemode->GetCurrentSlowValue();

		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle, this, &AAIProjectile::StopBeingSlowed, FMath::Abs(Gamemode->GetSlowEndTime() - GetWorld()->GetTimeSeconds()), false);
	}
}

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

void AAIProjectile::SetCurrentAI(AActor* AI)
{
	if (!ensure(AI != nullptr)) { return; }

	auto AIChar = Cast<ASuper_AI_Character>(AI);

	if (!ensure(AIChar != nullptr)) { return; }

	AICharacter = AIChar;
}

void AAIProjectile::StopBeingSlowed()
{
	CustomTimeDilation = 1;
}