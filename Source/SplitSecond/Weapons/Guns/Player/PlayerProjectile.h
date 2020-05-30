// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

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
	void OnBulletOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	bool bIsExplosive = false;
	bool bIsPiercing = false;
	bool bShouldBounce = false;
	int32 BounceNum = 3;
	int32 CurrentBounce = 0;

	APlayerProjectile();
	FVector CalculateReflectionVelocity(FVector ReflectorNormal);
	void ReflectProjectile(const FHitResult& Hit);

protected:
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* Bounce;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* Explosion;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* Homing;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* Piercing;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* ShieldImpact;

	UPROPERTY(EditAnywhere, Category = "FX")
	class UNiagaraSystem* ExplodingBulletFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Upgrade Settings")
	float ExplosionRadius = 300;
	
	/* How much speed is lost when projectile bounces off a surface */
	UPROPERTY(BlueprintReadWrite, Category = "Upgrade Settings")
	float BounceSpeedLoss = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* ExplosionRadiusCollision;
	UFUNCTION(BlueprintCallable, Category = "Damage Functions")
	void ApplyExplosionDamage();

	void SetExplosive(UNiagaraSystem*& SytemToSpawn, USoundBase*& SoundToPlay);
	void SetBounce(AActor* OtherActor, const FHitResult& Hit, USoundBase*& SoundToPlay, bool& bShouldDestroy);
	void PlayShieldSound(AActor* OtherActor, USoundBase*& SoundToPlay);
	void SetPiercing(AActor* OtherActor, USoundBase*& SoundToPlay, bool& bShouldDestroy);
};
