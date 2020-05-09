// This project falls under CC-BY-SA lisence

#include "PlayerShotgun.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../../Player/SplitSecondPlayerController.h"
#include "../../../AI/Super_AI_Character.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PlayerProjectile.h"

void APlayerShotgun::BeginPlay()
{
    Super::BeginPlay();
    CurrentBulletNum = DefaultBulletNum;
    CurrentBulletSpread = DefaultBulletSpread;
}
void APlayerShotgun::FireGun()
{
    if (ProjectileClass != NULL)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {
            if (CurrentAmmoCount > 0)
            {
                for (size_t i = 0; i < CurrentBulletNum; i++)
                {
                    FRotator SpawnRotation;

                    const auto Spread = CurrentBulletSpread;
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

                    // spawn the projectile at the muzzle
                    World->SpawnActor<APlayerProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
                }
                CurrentAmmoCount--;
                UE_LOG(LogTemp, Log, TEXT("Current Ammo Count: %f"), CurrentAmmoCount);

                GunMesh->CreateAndSetMaterialInstanceDynamic(1)->SetScalarParameterValue(TEXT("Emission Multiplier"), CurrentAmmoCount / CurrentAmmoMax);
                LastTimeFired = World->TimeSeconds;
            }
        }
    }
}