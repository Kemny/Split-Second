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

    /* The new dilation the boss will set */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Settings")
    float NewTimeDilation = 1;

	/* Amount of adds this boss spawns */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Settings")
	int32 AmountOfAddToSpawn = 4;

    /* Resets time dilation back to 1 */
    UFUNCTION(BlueprintCallable, Category = "Boss Functions")
    void ResetTimeDilation();
};
