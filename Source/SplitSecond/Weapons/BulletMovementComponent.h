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
    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
    //When this specific bullet gets slowed down
    float LocalTimeMultiplier = 1;
    float DefaultMaxSpeed;

    class ASplitSecondGameState* GameState;
};
