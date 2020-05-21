// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "../../SplitSecondProjectile.h"
#include "PlayerProjectile.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API APlayerProjectile : public ASplitSecondProjectile
{
	GENERATED_BODY()
	
public:
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;


	bool bIsExplosive = false;
	UPROPERTY(EditAnywhere) 
	bool bIsPiercing = false; // TODO this is not getting set by the gamemode that needs to be fixed
	bool bShouldBounce = false;
	int32 BounceNum = 3;
	int32 CurrentBounce = 0;

	/* If explosion upgrade is active this determines how long the effect is up for */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade Settings")
	float ExplosionUpTime = 1;

	/* If explosion upgrade is active this the explosion that will spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade Settings")
	TSubclassOf<class AProjectile_Explosion> ExplosionToSpawn;
protected:
	virtual void BeginPlay() override;
};
