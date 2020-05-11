// This project falls under CC-BY-SA lisence

#include "PlayerPistol.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../../Player/SplitSecondPlayerController.h"
#include "../../../AI/Super_AI_Character.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PlayerProjectile.h"

void APlayerPistol::FireGun()
{
    if (ProjectileClass != NULL)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {
            if (CurrentAmmoCount > 0)
            {
                FRotator SpawnRotation;

                const auto HitResult = World->GetFirstPlayerController<ASplitSecondPlayerController>()->LineTraceFromCamera(ECC_Camera);
                if (HitResult.GetActor())
                {
                    SpawnRotation = UKismetMathLibrary::FindLookAtRotation(GunMesh->GetSocketLocation(FName("MuzzleLocation")), HitResult.Location);
                }
                else
                {
                    SpawnRotation = CurrentPawn->GetControlRotation();
                }
                const FVector SpawnLocation = GunMesh->GetSocketLocation(FName("MuzzleLocation"));

                //Set Spawn Collision Handling Override
                FActorSpawnParameters ActorSpawnParams;
                ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

                // spawn the projectile at the muzzle
                World->SpawnActor<APlayerProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

                AfterPlayerFireGun(GunMesh);
            }
        }
    }
}