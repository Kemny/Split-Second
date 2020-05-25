// This project falls under CC-BY-SA lisence

#include "AIShotgun.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../../Player/SplitSecondPlayerController.h"
#include "../../../AI/Super_AI_Character.h"
#include "Engine/World.h"
#include "AIProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

#include "AIProjectile.h"

void AAIShotgun::FireGun()
{
    for (size_t i = 0; i < BulletNum; i++)
    {
        const auto Offset = FVector(FMath::RandRange(-BulletSpread, BulletSpread), FMath::RandRange(-BulletSpread, BulletSpread), FMath::RandRange(-BulletSpread, BulletSpread));
        AI_SpawnProjectile(Offset);
    }
}