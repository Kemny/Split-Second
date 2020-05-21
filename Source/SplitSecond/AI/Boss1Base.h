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

	void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	FTimerHandle CooldownTimer;

	void OnCooldownEnd();

	class AAIController* AIController = nullptr;
};
