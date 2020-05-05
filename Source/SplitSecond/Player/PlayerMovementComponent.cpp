// This project falls under CC-BY-SA lisence

#include "PlayerMovementComponent.h"
#include "GameFramework/Character.h"
#include "../SplitSecondGameState.h"
#include "Engine/World.h"

void UPlayerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}