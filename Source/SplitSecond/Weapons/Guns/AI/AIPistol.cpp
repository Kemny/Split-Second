// This project falls under CC-BY-SA lisence

#include "AIPistol.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../../Player/SplitSecondPlayerController.h"
#include "../../../AI/Super_AI_Character.h"
#include "AIProjectile.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

void AAIPistol::FireGun()
{
    if (ProjectileClass != NULL)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {
            const FRotator SpawnRotation = GunMesh->GetSocketRotation(FName("MuzzleLocation"));
            const FVector SpawnLocation = GunMesh->GetSocketLocation(FName("MuzzleLocation"));

            //Set Spawn Collision Handling Override
            FActorSpawnParameters ActorSpawnParams;
            ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			auto LocalCurrentProjectile = World->SpawnActor<ASplitSecondProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

			auto LocalCurrentAIProjectile = Cast<AAIProjectile>(LocalCurrentProjectile);

			if (LocalCurrentAIProjectile)
			{
				LocalCurrentAIProjectile->SetCurrentAI(GetCurrentPawn());
				LocalCurrentAIProjectile->ConstructEnemyProjectile();
			}
        }
    }
}
