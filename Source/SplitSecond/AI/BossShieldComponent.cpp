// This project falls under CC-BY-SA lisence


#include "BossShieldComponent.h"
#include "../AI/BossShieldBase.h"
#include "Engine/EngineTypes.h"
#include "Engine/World.h"

void UBossShieldComponent::ActivateShield()
{
	FActorSpawnParameters SpawnParms;

	CurrentShield = GetWorld()->SpawnActor<ABossShieldBase>(ShieldToSpawn, this->GetRelativeTransform(), SpawnParms);
	if (!ensure(CurrentShield != nullptr)) { return; }

	FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false);
	CurrentShield->AttachToComponent(this, AttachmentRules);
}

void UBossShieldComponent::DeactivateShield()
{
	if (!ensure(CurrentShield != nullptr)) { return; }

	CurrentShield->DeactivateShield();
}

