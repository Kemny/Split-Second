// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007


#include "TrapPlacer.h"
#include "Components/ChildActorComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UTrapPlacer::UTrapPlacer()
{
	TrapToPlace = CreateDefaultSubobject<UChildActorComponent>(TEXT("TrapToPlace"));
	TrapToPlace->SetupAttachment(this);
}