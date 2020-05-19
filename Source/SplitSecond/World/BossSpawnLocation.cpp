// This project falls under CC-BY-SA lisence


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

	ConstructorHelpers::FClassFinder<AActor> BP_Boss1_Class(TEXT("/Game/Blueprint/NPC/BP_Boss1"));
	if (BP_Boss1_Class.Class) Boss_1_Class = BP_Boss1_Class.Class;

	ConstructorHelpers::FClassFinder<AActor> BP_Boss2_Class(TEXT("/Game/Blueprint/NPC/BP_Boss2"));
	if (BP_Boss2_Class.Class) Boss_2_Class = BP_Boss2_Class.Class;

	ConstructorHelpers::FClassFinder<AActor> BP_Boss3_Class(TEXT("/Game/Blueprint/NPC/BP_Boss3"));
	if (BP_Boss3_Class.Class) Boss_3_Class = BP_Boss3_Class.Class;
}

TSubclassOf<AActor> UBossSpawnLocation::GetCurrentTypeClass() const
{
	switch (SpawnType)
	{
	case Boss_1:
		return Boss_1_Class;
		break;
	case Boss_2:
		return Boss_2_Class;
		break;
	case Boss_3:
		return Boss_3_Class;
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

