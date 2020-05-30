// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007


#include "BossSpawnLocation.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UBossSpawnLocation::UBossSpawnLocation()
{
	SetVisibility(false);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComponent->SetupAttachment(this);
	BoxComponent->SetBoxExtent(FVector(32, 32, 95));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore);

	ConstructorHelpers::FClassFinder<AActor> BP_Boss1_Class(TEXT("/Game/Blueprint/NPC/BP_Boss"));
	if (BP_Boss1_Class.Class) Boss_1_Class = BP_Boss1_Class.Class;
}

TSubclassOf<AActor> UBossSpawnLocation::GetCurrentTypeClass() const
{
	switch (SpawnType)
	{
	case Boss_1:
		return Boss_1_Class;
		break;
	default:
		return TSubclassOf<AActor>();
		break;
	}
}

FVector UBossSpawnLocation::GetBoxCenter() const
{
	if (!ensure(BoxComponent != nullptr)) { return FVector(0); }

	return BoxComponent->Bounds.GetBox().GetCenter();
}

