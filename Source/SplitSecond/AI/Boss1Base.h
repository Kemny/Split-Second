// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "../AI/Super_Boss.h"
#include "Boss1Base.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API ABoss1Base : public ASuper_Boss
{
	GENERATED_BODY()

public:

	/* Delay before the shoots his gun */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Settings")
	float GunShotDelay = 3;

	/* Multiplier used for dashing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Settings")
	float DashMultiplier = 1500;

	/* Timer length of dash cooldown */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Settings")
	float DashCooldown = 2;

	/* Whether or not dash is currently on cooldown */
	UPROPERTY(BlueprintReadOnly, Category = "Boss Settings")
	bool bDashOnCooldown = false;

	/* Starts dash cooldown timer */
	UFUNCTION(BlueprintCallable, Category = "Boss Settings")
	void StartDashCooldown();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* DashCollision;

	/* Impact normal on Dash Collision */
	UPROPERTY(BlueprintReadOnly)
	FVector BulletOverlapNormal = FVector(0);

	ABoss1Base();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle CooldownTimer;

	void OnCooldownEnd();

	class AAIController* Controller = nullptr;

	FVector GetLaunchDirection(FVector Normal);
};
