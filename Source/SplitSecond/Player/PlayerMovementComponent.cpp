// This project falls under CC-BY-SA lisence

#include "PlayerMovementComponent.h"
#include "GameFramework/Character.h"
#include "../SplitSecondGameState.h"
#include "Engine/World.h"

void UPlayerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UPlayerMovementComponent::DoJump(bool bReplayingMoves)
{
	auto* GameState = GetWorld()->GetGameState<ASplitSecondGameState>();
	if (!ensure(GameState != nullptr)) { return false; }

	if (CharacterOwner && CharacterOwner->CanJump())
	{
		// Don't jump if we can't move up/down.
		if (!bConstrainToPlane || FMath::Abs(PlaneConstraintNormal.Z) != 1.f)
		{
			/// This is jump height
			Velocity.Z = FMath::Max(Velocity.Z, JumpZVelocity);

			SetMovementMode(MOVE_Falling);
			return true;
		}
	}

	return false;
}