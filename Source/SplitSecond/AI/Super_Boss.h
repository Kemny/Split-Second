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

    /* Resets time dilation back to 1 */
    UFUNCTION(BlueprintCallable, Category = "Boss Functions")
    void ResetTimeDilation();

	void ScaleEnemyHealth(float BaseValue) override;
	void ScaleEnemyDamage(float BaseValue) override;
};
