// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile_Explosion.generated.h"

UCLASS()
class SPLITSECOND_API AProjectile_Explosion : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_Explosion();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* ExplosionRadiusCollision;

	UFUNCTION(BlueprintCallable, Category = "Damage Functions")
	void ApplyExplosionDamage(float Damage, float ExplosionUpTime);

	TArray<AActor*> ActorsToDamage;
};
