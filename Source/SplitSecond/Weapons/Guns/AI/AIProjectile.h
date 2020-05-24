// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "../../SplitSecondProjectile.h"
#include "AIProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API AAIProjectile : public ASplitSecondProjectile
{
	GENERATED_BODY()

public:
	void BeginPlay() override;
	UFUNCTION() void StopBeingSlowed();
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	/* Called on begin play */
	UFUNCTION(BlueprintImplementableEvent)
	void ConstructEnemyProjectile();

	void SetCurrentAI(AActor* AI);

	/* The AI that spawned this projectile */
	UPROPERTY(BlueprintReadWrite)
	class ASuper_AI_Character* AICharacter = nullptr;
};
