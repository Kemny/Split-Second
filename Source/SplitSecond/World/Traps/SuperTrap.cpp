// This project falls under CC-BY-SA lisence


#include "SuperTrap.h"
#include "Components/BoxComponent.h"

ASuperTrap::ASuperTrap()
{
	TraceComp = CreateDefaultSubobject<UBoxComponent>(TEXT("TraceComp"));
	TraceComp->SetupAttachment(RootComponent);
	TraceComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TraceComp->SetCollisionObjectType(ECC_WorldDynamic);
	TraceComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	TraceComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	TraceComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
}
