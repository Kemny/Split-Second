// This project falls under CC-BY-SA lisence

#include "PlayerBow.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../../Player/SplitSecondPlayerController.h"
#include "../../../AI/Super_AI_Character.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PlayerProjectile.h"

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
	bIsDrawingBow = false;
	bIsHeld = false;
	BowDrawPrecentage = 0;
    
    if (CurrentAmmoCount > 0)
    {
        FireGun();
    }
}

void APlayerBow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDrawingBow && !bIsHeld)
	{
		if (BowDrawPrecentage >= 1)
		{
            UE_LOG(LogTemp, Log, TEXT("Bow Loaded"));
			bIsHeld = true;
		}
		BowDrawPrecentage += BowDrawSpeed;
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
            World->SpawnActor<APlayerProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
            
            CurrentAmmoCount--;
            UE_LOG(LogTemp, Log, TEXT("Current Ammo Count: %f"), CurrentAmmoCount);

            BowMesh->CreateAndSetMaterialInstanceDynamic(1)->SetScalarParameterValue(TEXT("Emission Multiplier"), CurrentAmmoCount / CurrentAmmoMax);
            LastTimeFired = World->TimeSeconds;
        }
    }
}