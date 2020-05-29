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
	void OnBulletOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
