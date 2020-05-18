// This project falls under CC-BY-SA lisence


#include "TrapPlacer.h"
#include "Components/ChildActorComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UTrapPlacer::UTrapPlacer()
{
	TrapToPlace = CreateDefaultSubobject<UChildActorComponent>(TEXT("TrapToPlace"));
	TrapToPlace->SetupAttachment(this);
}