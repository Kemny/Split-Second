// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "PlayerPistol.h"
#include "Components/StaticMeshComponent.h"

void APlayerPistol::FireGun()
{
    if (ProjectileClass != NULL)
    {
        // spawn the projectile at the muzzle
        if (auto Spawned = Player_SpawnProjectile(ProjectileClass, GunMesh->GetSocketLocation(FName("MuzzleLocation")), FindPlayerBulletRotation(GunMesh)))
        {
            AfterPlayerFireGun();
        }
    }
}