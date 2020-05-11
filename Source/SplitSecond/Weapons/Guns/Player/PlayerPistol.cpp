// This project falls under CC-BY-SA lisence

#include "PlayerPistol.h"
#include "GameFramework/Character.h"
#include "../../../Player/SplitSecondPlayerController.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "../../../SplitSecondPlayerState.h"

void APlayerPistol::FireGun()
{
    if (!ensure(PlayerState != nullptr)) { return; }

    if (ProjectileClass != NULL)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {
            if (PlayerState->CurrentStats.Ammo > 0)
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
                if (auto Spawned = Player_SpawnProjectile(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams))
                {
                    AfterPlayerFireGun(GunMesh);
                }
            }
        }
    }
}