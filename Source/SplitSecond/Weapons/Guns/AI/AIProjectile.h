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
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};
