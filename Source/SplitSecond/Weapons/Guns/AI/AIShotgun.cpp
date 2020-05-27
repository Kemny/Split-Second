// This project falls under CC-BY-SA lisence

#include "AIShotgun.h"
#include "Kismet/GameplayStatics.h"

void AAIShotgun::FireGun()
{
    if (!ensure(ShootSound != nullptr)) { return; }
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, GetActorLocation());

    for (size_t i = 0; i < BulletNum; i++)
    {
        const auto Offset = FVector(FMath::RandRange(-BulletSpread, BulletSpread), FMath::RandRange(-BulletSpread, BulletSpread), FMath::RandRange(-BulletSpread, BulletSpread));
        AI_SpawnProjectile(Offset);
    }
}