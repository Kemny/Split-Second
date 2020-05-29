// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "PlayerShotgun.h"
#include "Components/StaticMeshComponent.h"
#include "../../../SplitSecondPlayerState.h"

void APlayerShotgun::FireGun()
{
    if (!ensure(PlayerState != nullptr)) { return; }
    
    if (ProjectileClass != NULL)
    {
        const auto Spread = PlayerState->CurrentStats.BulletSpread;
        for (size_t i = 0; i < PlayerState->CurrentStats.BulletNum; i++)
        {
            const auto Offset = FVector(FMath::RandRange(-Spread, Spread), FMath::RandRange(-Spread, Spread), FMath::RandRange(-Spread, Spread));
            if (auto Spawned = Player_SpawnProjectile(ProjectileClass, GunMesh->GetSocketLocation(FName("MuzzleLocation")), FindPlayerBulletRotation(GunMesh, Offset)))
            {
                AfterPlayerFireGun();
            }
        }
    }
}