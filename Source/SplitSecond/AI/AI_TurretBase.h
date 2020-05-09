// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "../AI/Super_AI_Character.h"
#include "AI_TurretBase.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API AAI_TurretBase : public ASuper_AI_Character
{
	GENERATED_BODY()

public:

  AAI_TurretBase();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
  class UAI_TraceComponent* AITraceComp;

  /* When the turret trace stops hitting the player this the amount time it takes to stop shooting the player  */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret Settings")
  float DeaggroDelay = 4;

  /* Time it takes for the turret to rotate to player */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret Settings")
  float RotationTime = 1;
	
};
