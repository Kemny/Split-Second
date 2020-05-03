// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SplitSecond_AI_Controller.generated.h"

class ASuper_AI_Character;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class SPLITSECOND_API ASplitSecond_AI_Controller : public AAIController
{
	GENERATED_BODY()

public:

  /* Fires the current AI's gun */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  void FireGun();
  
  UFUNCTION(BlueprintPure, Category = "Controller Functions")
  ASuper_AI_Character* GetCurrentAI();

  virtual void OnPossess(APawn* InPawn) override;

private:

  ASuper_AI_Character* CurrentControlledAI;

  UBehaviorTree* GetBehaviorTreeFromAI();

};
