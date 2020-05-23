// This project falls under CC-BY-SA lisence

#include "AIShotgun.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../../Player/SplitSecondPlayerController.h"
#include "../../../AI/Super_AI_Character.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "AIProjectile.h"

void AAIShotgun::FireGun()
{
    if (ProjectileClass != NULL)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {
            for (size_t i = 0; i < DefaultBulletNum; i++)
            {
                const FRotator SpawnRotation = GunMesh->GetSocketRotation(FName("MuzzleLocation"));
                const FVector SpawnLocation = GunMesh->GetSocketLocation(FName("MuzzleLocation"));

                //Set Spawn Collision Handling Override
                FActorSpawnParameters ActorSpawnParams;
                ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

                // spawn the projectile at the muzzle
                World->SpawnActor<ASplitSecondProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
            }
        }
    }
}