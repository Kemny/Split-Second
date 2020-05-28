// This project falls under CC-BY-SA lisence

#include "AIShotgun.h"
#include "Kismet/GameplayStatics.h"

void AAIShotgun::FireGun()
{
    if (!ensure(FireSouns.Num() > 0)) { return; }
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSouns[FMath::RandRange(0, FireSouns.Num() - 1)], GetActorLocation());

    for (size_t i = 0; i < BulletNum; i++)
    {
        const auto Offset = FVector(FMath::RandRange(-BulletSpread, BulletSpread), FMath::RandRange(-BulletSpread, BulletSpread), FMath::RandRange(-BulletSpread, BulletSpread));
        AI_SpawnProjectile(Offset);
    }
}