// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "../AI/Super_AI_Character.h"
#include "Super_Boss.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API ASuper_Boss : public ASuper_AI_Character
{
	GENERATED_BODY()

public:
	void GetSlowed(float SlowTime, float SlowAmmount) override;

	void ScaleEnemyHealth(float BaseValue) override;
	void ScaleEnemyDamage(float BaseValue) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	float SlowDivider = 2;
};
