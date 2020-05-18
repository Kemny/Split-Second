// This project falls under CC-BY-SA lisence


#include "Boss1Base.h"
#include "../AI/BossShieldComponent.h"

ABoss1Base::ABoss1Base()
{
	ShieldComp = CreateDefaultSubobject<UBossShieldComponent>(TEXT("Shield Comp"));
	ShieldComp->SetupAttachment(RootComponent);
}

void ABoss1Base::ActivateShield()
{
	if (!ensure(ShieldComp != nullptr)) { return; }
	ShieldComp->ActivateShield();
}

void ABoss1Base::DeactivateShield()
{
	if (!ensure(ShieldComp != nullptr)) { return; }
	ShieldComp->DeactivateShield();
}