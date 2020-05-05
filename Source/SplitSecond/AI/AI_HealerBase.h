// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "../AI/Super_AI_Character.h"
#include "AI_HealerBase.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API AAI_HealerBase : public ASuper_AI_Character
{
	GENERATED_BODY()
	
public:
  
  AAI_HealerBase();
  
  /* Amount to heal the damaged enemy */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healer Settings")
  float HealAmount;

  /* If healer finds a NPC with health below this threshold then that NPC will become it's target to heal */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healer Settings")
  float HealthThreshold;

  /* Delay before the healer starts looking for a new target to heal */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healer Settings")
  float TargetingDelay;
};
