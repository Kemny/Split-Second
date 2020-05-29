// This project falls under CC-BY-SA lisence

#include "AIProjectile.h"
#include "Engine/World.h"
#include "../../../AI/Super_AI_Character.h"
#include "../../../SplitSecondGameMode.h"

void AAIProjectile::BeginPlay()
{
	Super::BeginPlay();

	auto Gamemode = GetWorld()->GetAuthGameMode<ASplitSecondGameMode>();
	if (!ensure(Gamemode != nullptr)) { return; }

	if (Gamemode->GetIsGameSlowed())
	{
		Gamemode->AddActorToSlowedArray(this);
	}
}
void AAIProjectile::OnBulletOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ASuper_AI_Character>()) Destroy();

	Super::OnBulletOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}
