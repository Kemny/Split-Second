// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BulletMovementComponent.generated.h"

/**
 * Projectile Movement Component Override
 * This one can be slowed down by time
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class SPLITSECOND_API UBulletMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:
    UBulletMovementComponent();
    void BeginPlay() override;

    FVector ComputeVelocity(FVector InitialVelocity, float DeltaTime) const override;

protected:
    

private:
    float LocalTimeMultiplier;
    class ASplitSecondGameState* GameState;
};
