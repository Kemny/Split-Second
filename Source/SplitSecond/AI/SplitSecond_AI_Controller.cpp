// This project falls under CC-BY-SA lisence


#include "SplitSecond_AI_Controller.h"
#include "../AI/Super_AI_Character.h"

void ASplitSecond_AI_Controller::FireGun()
{
  if (!ensure(CurrentControlledAI != nullptr)) { return; }

  CurrentControlledAI->FireGun();
}

void ASplitSecond_AI_Controller::BeginPlay()
{
  Super::BeginPlay();

  CurrentControlledAI = Cast<ASuper_AI_Character>(GetPawn());
}

ASuper_AI_Character* ASplitSecond_AI_Controller::GetCurrentAI()
{
  return CurrentControlledAI;
}
