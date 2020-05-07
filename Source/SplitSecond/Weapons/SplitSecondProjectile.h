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
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	FORCEINLINE class UBulletMovementComponent* GetProjectileMovement() const { return BulletMovement; }
	FORCEINLINE class UStaticMeshComponent* GetBulletMesh() const { return BulletMesh; }

	bool GetIsSlowed() const { return bIsSlowed; }
	void GetSlowed(float SlowTime, float SlowAmmount);
	UFUNCTION() void StopBeingSlowed();

  UFUNCTION(BlueprintImplementableEvent, Category = "Bullet Events")
  void BulletOnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	bool bIsSlowed;
};

