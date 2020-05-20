// This project falls under CC-BY-SA lisence


#include "Boss1Base.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"

ABoss1Base::ABoss1Base()
{
	DashCollision = CreateDefaultSubobject<USphereComponent>(TEXT("DashCollision"));
	DashCollision->SetupAttachment(RootComponent);
}

void ABoss1Base::StartDashCooldown()
{
	bDashOnCooldown = true;
	GetWorldTimerManager().SetTimer(CooldownTimer, this, &ABoss1Base::OnCooldownEnd, DashCooldown, false);
}

void ABoss1Base::OnCooldownEnd()
{
	bDashOnCooldown = false;
}

void ABoss1Base::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		BulletOverlapNormal = SweepResult.Normal;
		UE_LOG(LogTemp, Log, TEXT("Normal %s"), *BulletOverlapNormal.ToString())

		if (!bDashOnCooldown)
		{
			auto DashDirection = GetLaunchDirection(BulletOverlapNormal) * DashMultiplier;
			UE_LOG(LogTemp, Log, TEXT("Direction %s"), *DashDirection.ToString())
			
			if (!ensure(Controller != nullptr)) { return; }
			Controller->BrainComponent->StopLogic(TEXT("Starting Dash"));

			LaunchCharacter(DashDirection, true, true);
			BulletOverlapNormal = FVector(0);

			Controller->BrainComponent->RestartLogic();
			StartDashCooldown();
		}
	}
}

void ABoss1Base::BeginPlay()
{
	Super::BeginPlay();

	DashCollision->OnComponentBeginOverlap.AddDynamic(this, &ABoss1Base::OnOverlapBegin);

	Controller = Cast<AAIController>(GetController());
}

FVector ABoss1Base::GetLaunchDirection(FVector Normal)
{
	return FVector(0);
}

