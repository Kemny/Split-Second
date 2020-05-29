// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "PlayerBow.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../../SplitSecondPlayerState.h"

APlayerBow::APlayerBow()
{
	BowMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bow"));
	BowMesh->SetupAttachment(GetRootComponent());
}

void APlayerBow::OnInputPressed_Implementation()
{
    bIsDrawingBow = true;
}

void APlayerBow::OnInputReleased_Implementation()
{
    if (!ensure(PlayerState != nullptr)) { return; }

    if (BowDrawPrecentage > PlayerState->CurrentStats.MinimalDrawValue)
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
        // spawn the projectile at the muzzle
        if (auto Spawned = Player_SpawnProjectile(ProjectileClass, BowMesh->GetSocketLocation(FName("MuzzleLocation")), FindPlayerBulletRotation(BowMesh)))
        {
            AfterPlayerFireGun();
        }
    }
}