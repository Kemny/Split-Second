// This project falls under CC-BY-SA lisence

#include "AIProjectile.h"
#include "../../../Player/PlayerCharacter.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "../../../SplitSecondGameMode.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

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

	SetActorRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()));
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

void AAIProjectile::StopBeingSlowed()
{
	CustomTimeDilation = 1;
}