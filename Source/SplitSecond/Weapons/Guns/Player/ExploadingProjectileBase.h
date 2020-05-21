// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "PlayerProjectile.h"
#include "ExploadingProjectileBase.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API AExploadingProjectileBase : public APlayerProjectile
{
	GENERATED_BODY()

public:

	AExploadingProjectileBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* ExplosionSphere;

	virtual void OnBulletHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
};
