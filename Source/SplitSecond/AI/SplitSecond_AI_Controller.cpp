// This project falls under CC-BY-SA lisence


#include "SplitSecond_AI_Controller.h"
#include "BehaviorTree/BehaviorTree.h"
#include "../AI/Super_AI_Character.h"

void ASplitSecond_AI_Controller::FireGun()
{
  if (!ensure(CurrentControlledAI != nullptr)) { return; }

  CurrentControlledAI->FireGun();
}

UBehaviorTree* ASplitSecond_AI_Controller::GetBehaviorTreeFromAI()
{
  if (CurrentControlledAI)
  {
    if (CurrentControlledAI->BehaviorTreeToUse)
    {
      return CurrentControlledAI->BehaviorTreeToUse;
    } 
    else
    {
      return nullptr;
    }
  }
  else
  {
    return nullptr;
  }
}

void ASplitSecond_AI_Controller::OnPossess(APawn* InPawn)
{
  Super::OnPossess(InPawn);

  if (InPawn)
  {
    CurrentControlledAI = Cast<ASuper_AI_Character>(InPawn);

    UBehaviorTree* TreeToRun = GetBehaviorTreeFromAI();

    if (TreeToRun)
    {
      RunBehaviorTree(TreeToRun);
    }
  }
}

ASuper_AI_Character* ASplitSecond_AI_Controller::GetCurrentAI()
{
  return CurrentControlledAI;
}
