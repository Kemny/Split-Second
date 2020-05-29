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
	class UProjectileMovementComponent* ProjectileMovement;

public:
	ASplitSecondProjectile();

	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	FORCEINLINE class UStaticMeshComponent* GetBulletMesh() const { return BulletMesh; }

	UFUNCTION()
	virtual void OnBulletOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UPROPERTY(BlueprintReadWrite)
	float Damage = 10;

	UPROPERTY(EditAnywhere, Category = "FX")
	class UNiagaraSystem* DefaultCollisionParticle;

public:
	void SetDamage(float NewDamage) { Damage = NewDamage; }

};

