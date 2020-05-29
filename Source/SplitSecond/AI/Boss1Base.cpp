// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007


#include "Boss1Base.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"

void ABoss1Base::StartDashCooldown()
{
	bDashOnCooldown = true;
	GetWorldTimerManager().SetTimer(CooldownTimer, this, &ABoss1Base::OnCooldownEnd, DashCooldown, false);
}

void ABoss1Base::OnCooldownEnd()
{
	bDashOnCooldown = false;
}

void ABoss1Base::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(GetController());
}

void ABoss1Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (!bDashOnCooldown)
	//{
	//	auto DashDirection = GetActorRightVector() * DashMultiplier;
	//	AIController->BrainComponent->StopLogic(TEXT("Started Dash"));

	//	LaunchCharacter(DashDirection, true, true);

	//	AIController->BrainComponent->RestartLogic();
	//	StartDashCooldown();
	//}
}