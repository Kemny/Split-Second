// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "PlayerMovementComponent.h"
#include "GameFramework/Character.h"
#include "../SplitSecondGameState.h"
#include "Engine/World.h"

void UPlayerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}