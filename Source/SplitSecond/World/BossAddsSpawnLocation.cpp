// This project falls under CC-BY-SA lisence

#include "BossAddsSpawnLocation.h"
#include "EnemySpawnLocation.h"
#include "Components/BoxComponent.h"
#include "../AI/Super_AI_Character.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UBossAddsSpawnLocation::UBossAddsSpawnLocation()
{
	SetVisibility(false);

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComponent->SetupAttachment(this);
	BoxComponent->SetBoxExtent(FVector(32, 32, 95));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UBossAddsSpawnLocation::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &UBossAddsSpawnLocation::OnOverlapEnd);

	ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Charger_Class(TEXT("/Game/Blueprint/NPC/BP_Charger"));
	if (BP_Enemy_Charger_Class.Class) Enemy_Charger_Class = BP_Enemy_Charger_Class.Class;

	ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Shooter_Class(TEXT("/Game/Blueprint/NPC/BP_Shooter"));
	if (BP_Enemy_Shooter_Class.Class) Enemy_Shooter_Class = BP_Enemy_Shooter_Class.Class;
}

TSubclassOf<AActor> UBossAddsSpawnLocation::GetCurrentTypeClass() const
{
	switch (SpawnType)
	{
	case Boss_Enemy_Charger:
		return Enemy_Charger_Class;
		break;
	case Boss_Enemy_Healer:
		return Enemy_Healer_Class;
		break;
	case Boss_Enemy_Shooter:
		return Enemy_Shooter_Class;
		break;
	default:
		return TSubclassOf<AActor>();
		break;
	}
}

FVector UBossAddsSpawnLocation::GetBoxCenter() const
{
	if (!ensure(BoxComponent != nullptr)) { return FVector(0); }

	return BoxComponent->Bounds.GetBox().GetCenter();
}

bool UBossAddsSpawnLocation::HasEnemyInBounds()
{
	return bEnemyInBounds;
}

void UBossAddsSpawnLocation::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (OtherActor->IsA<ASuper_AI_Character>())
		{
			bEnemyInBounds = true;
		}
	}
}

void UBossAddsSpawnLocation::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (OtherActor->IsA<ASuper_AI_Character>())
		{
			bEnemyInBounds = false;
		}
	}
}