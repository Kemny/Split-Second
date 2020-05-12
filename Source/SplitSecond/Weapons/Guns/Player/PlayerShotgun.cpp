// This project falls under CC-BY-SA lisence

#include "PlayerShotgun.h"
#include "GameFramework/Character.h"
#include "../../../Player/SplitSecondPlayerController.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "../../../SplitSecondPlayerState.h"

void APlayerShotgun::FireGun()
{
    if (!ensure(PlayerState != nullptr)) { return; }

    if (ProjectileClass != NULL)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {
            if (PlayerState->CurrentStats.Ammo > 0)
            {
                for (size_t i = 0; i < PlayerState->CurrentStats.BulletNum; i++)
                {
                    FRotator SpawnRotation;

                    const auto Spread = PlayerState->CurrentStats.BulletSpread;
                    const auto Offset = FVector(FMath::RandRange(-Spread, Spread), FMath::RandRange(-Spread, Spread), FMath::RandRange(-Spread, Spread));
                    const auto HitResult = World->GetFirstPlayerController<ASplitSecondPlayerController>()->LineTraceFromCamera(ECC_Camera, Offset);
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

                    Player_SpawnProjectile(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
                    AfterPlayerFireGun(GunMesh);
                }
            }
        }
    }
}