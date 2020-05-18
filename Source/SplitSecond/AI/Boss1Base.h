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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBossShieldComponent* ShieldComp;

	/* Active boss shield */
	UFUNCTION(BlueprintCallable, Category = "Boss Functions")
	void ActivateShield();

	/* Deactivate boss shield */
	UFUNCTION(BlueprintCallable, Category = "Boss Functions")
	void DeactivateShield();
	
	ABoss1Base();
};
