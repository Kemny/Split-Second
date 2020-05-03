// This project falls under CC-BY-SA lisence

#include "BulletMovementComponent.h"
#include "../SplitSecondGameState.h"
#include "Engine/World.h"


UBulletMovementComponent::UBulletMovementComponent()
{
	bShouldBounce = false;
	ProjectileGravityScale = false;
}

void UBulletMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetWorld()->GetGameState<ASplitSecondGameState>();
	if (!ensure(GameState != nullptr)) return;
}

FVector UBulletMovementComponent::ComputeVelocity(FVector InitialVelocity, float DeltaTime) const
{
	if (!ensure(GameState != nullptr)) { return FVector(); }

	const FVector Acceleration = ComputeAcceleration(InitialVelocity, DeltaTime) * GameState->GetGlobalTimeMultiplier();
	FVector NewVelocity = InitialVelocity + (Acceleration * DeltaTime);

	return LimitVelocity(NewVelocity);
}