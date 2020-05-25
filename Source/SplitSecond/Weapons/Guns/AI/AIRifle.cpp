// This project falls under CC-BY-SA lisence


#include "AIRifle.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../../Player/SplitSecondPlayerController.h"
#include "../../../AI/Super_AI_Character.h"
#include "Engine/World.h"
#include "AIProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

void AAIRifle::FireGun()
{
	AI_SpawnProjectile();
}
