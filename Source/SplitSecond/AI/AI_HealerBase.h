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

};
