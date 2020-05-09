// This project falls under CC-BY-SA lisence

#include "BulletMovementComponent.h"
#include "../SplitSecondGameState.h"
#include "Engine/World.h"
#include "SplitSecondProjectile.h"

UBulletMovementComponent::UBulletMovementComponent()
{
	bTickBeforeOwner = false;

	bShouldBounce = false;
	ProjectileGravityScale = false;
}

void UBulletMovementComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UBulletMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
