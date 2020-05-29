// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007


#include "SuperTrap.h"
#include "Components/BoxComponent.h"

ASuperTrap::ASuperTrap()
{
	TraceComp = CreateDefaultSubobject<UBoxComponent>(TEXT("TraceComp"));
	RootComponent = TraceComp;
	TraceComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TraceComp->SetCollisionObjectType(ECC_WorldDynamic);
	TraceComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	TraceComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	TraceComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
}
