// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

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

protected:
	UPROPERTY(EditDefaultsOnly)
	float SlowDivider = 2;
};
