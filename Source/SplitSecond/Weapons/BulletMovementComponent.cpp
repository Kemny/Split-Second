// This project falls under CC-BY-SA lisence

#include "BulletMovementComponent.h"
#include "../SplitSecondGameState.h"
#include "Engine/World.h"


UBulletMovementComponent::UBulletMovementComponent()
{
	bTickBeforeOwner = false;

	bShouldBounce = false;
	ProjectileGravityScale = false;
}

void UBulletMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	GameState = GetWorld()->GetGameState<ASplitSecondGameState>();
	if (!ensure(GameState != nullptr)) return;

	DefaultMaxSpeed = MaxSpeed;
}

void UBulletMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!ensure(GameState != nullptr)) return;
	
	Velocity += Velocity.GetSafeNormal() * GameState->GetGlobalTimeMultiplier() * InitialSpeed * LocalTimeMultiplier;
	MaxSpeed = DefaultMaxSpeed * GameState->GetGlobalTimeMultiplier();

}
