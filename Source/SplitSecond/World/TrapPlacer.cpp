// This project falls under CC-BY-SA lisence


#include "TrapPlacer.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UTrapPlacer::UTrapPlacer()
{
	SetVisibility(false);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComponent->SetupAttachment(this);
	BoxComponent->SetBoxExtent(FVector(100.0, 100.0, 10));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FClassFinder<AActor> BP_Trap_Spike_Class(TEXT("/Game/Blueprint/World/Traps/BP_SpikeTrap"));
	if (BP_Trap_Spike_Class.Class) Trap_Spike_Class = BP_Trap_Spike_Class.Class;

	ConstructorHelpers::FClassFinder<AActor> BP_Trap_Fire_Class(TEXT("/Game/Blueprint/World/Traps/BP_FireTrap"));
	if (BP_Trap_Fire_Class.Class) Trap_Fire_Class = BP_Trap_Fire_Class.Class;

	ConstructorHelpers::FClassFinder<AActor> BP_Trap_Cannon_Class(TEXT("/Game/Blueprint/World/Traps/BP_CannonTrap"));
	if (BP_Trap_Cannon_Class.Class) Trap_Cannon_Class = BP_Trap_Cannon_Class.Class;
}

TSubclassOf<AActor> UTrapPlacer::GetCurrentTypeClass() const
{
	switch (SpawnType)
	{
	case Trap_Fire:
		return Trap_Fire_Class;
		break;
	case Trap_Spike:
		return Trap_Spike_Class;
	case Trap_Cannon:
		return Trap_Cannon_Class;
	default:
		return TSubclassOf<AActor>();
		break;
	}
}

FVector UTrapPlacer::GetBoxCenter() const
{
	if (!ensure(BoxComponent != nullptr)) { return FVector(0); }

	return BoxComponent->Bounds.GetBox().GetCenter(); 
}