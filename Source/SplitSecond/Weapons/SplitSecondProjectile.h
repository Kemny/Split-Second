// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplitSecondProjectile.generated.h"

UCLASS(config=Game)
class ASplitSecondProjectile : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* LineTraceComp;
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class UStaticMeshComponent* BulletMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UBulletMovementComponent* BulletMovement;

public:
	ASplitSecondProjectile();

	/** called when projectile hits something */
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) PURE_VIRTUAL(&ASplitSecondProjectile::OnHit, );

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	FORCEINLINE class UBulletMovementComponent* GetProjectileMovement() const { return BulletMovement; }
	FORCEINLINE class UStaticMeshComponent* GetBulletMesh() const { return BulletMesh; }

	void GetSlowed(float SlowTime, float SlowAmmount);
	UFUNCTION() void StopBeingSlowed();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bullet Events")
	void OnBulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	virtual void OnBulletHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Bullet Events")
	void OnBulletOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnBulletOverlap_Implementation(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void CalcReflection(const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float DamageValue = 10;

	/* How much speed is lost when projectile bounces off a surface */
	UPROPERTY(BlueprintReadWrite, Category = "Projectile")
	float BounceSpeedLoss = 1;

protected:
	class UNiagaraSystem* NiagaraSystem;
	FTimerHandle SlowTimerHandle;
};

