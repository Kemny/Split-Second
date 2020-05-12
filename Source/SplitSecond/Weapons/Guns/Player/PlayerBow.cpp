// This project falls under CC-BY-SA lisence

#include "PlayerBow.h"
#include "PlayerProjectile.h"
#include "GameFramework/Character.h"
#include "../../../Player/SplitSecondPlayerController.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../../SplitSecondPlayerState.h"
#include "PlayerProjectile.h"
#include "../../BulletMovementComponent.h"

APlayerBow::APlayerBow()
{
	BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bow"));
	BowMesh->SetupAttachment(GetRootComponent());
}

void APlayerBow::OnInputPressed_Implementation()
{
    if (!ensure(PlayerState != nullptr)) { return; }
    if (PlayerState->CurrentStats.Ammo > 0)
    {
        bIsDrawingBow = true;
    }
}

void APlayerBow::OnInputReleased_Implementation()
{
    if (!ensure(PlayerState != nullptr)) { return; }

    if (PlayerState->CurrentStats.Ammo > 0 && BowDrawPrecentage > PlayerState->CurrentStats.MinimalDrawValue)
    {
        FireGun();
    }

    bIsHeld = false;
    bIsDrawingBow = false;
    BowDrawPrecentage = 0;
}

void APlayerBow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!ensure(PlayerState != nullptr)) { return; }

	if (bIsDrawingBow && !bIsHeld)
	{
		if (BowDrawPrecentage >= 1)
		{
            UE_LOG(LogTemp, Log, TEXT("Bow Loaded"));
            BowDrawPrecentage = 1;
            bIsDrawingBow = false;
			bIsHeld = true;
		}
		BowDrawPrecentage += PlayerState->CurrentStats.BowDrawSpeed;
	}
}

void APlayerBow::FireGun()
{
    if (ProjectileClass != NULL)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {
            FRotator SpawnRotation;

            const auto HitResult = World->GetFirstPlayerController<ASplitSecondPlayerController>()->LineTraceFromCamera(ECC_Camera);
            if (HitResult.GetActor())
            {
                SpawnRotation = UKismetMathLibrary::FindLookAtRotation(BowMesh->GetSocketLocation(FName("MuzzleLocation")), HitResult.Location);
            }
            else
            {
                SpawnRotation = CurrentPawn->GetControlRotation();
            }
            const FVector SpawnLocation = BowMesh->GetSocketLocation(FName("MuzzleLocation"));

            //Set Spawn Collision Handling Override
            FActorSpawnParameters ActorSpawnParams;
            ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

            // spawn the projectile at the muzzle
            if (auto Spawned = Player_SpawnProjectile(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams))
            {
                Spawned->GetProjectileMovement()->ProjectileGravityScale = FMath::Abs(BowDrawPrecentage - 1);
                AfterPlayerFireGun(BowMesh);
            }
        }
    }
}