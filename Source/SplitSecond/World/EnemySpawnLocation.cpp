// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007


#include "EnemySpawnLocation.h"
#include "Components/BoxComponent.h"
#include "../AI/Super_AI_Character.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UEnemySpawnLocation::UEnemySpawnLocation()
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

    BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &UEnemySpawnLocation::OnOverlapBegin);
    BoxComponent->OnComponentEndOverlap.AddDynamic(this, &UEnemySpawnLocation::OnOverlapEnd);
    
    ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Charger_Class(TEXT("/Game/Blueprint/NPC/BP_Charger"));
    if (BP_Enemy_Charger_Class.Class) Enemy_Charger_Class = BP_Enemy_Charger_Class.Class;

    ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Shooter_Class(TEXT("/Game/Blueprint/NPC/BP_Shooter"));
    if (BP_Enemy_Shooter_Class.Class) Enemy_Shooter_Class = BP_Enemy_Shooter_Class.Class;

    ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Turret_Class(TEXT("/Game/Blueprint/NPC/BP_Turret"));
    if (BP_Enemy_Turret_Class.Class) Enemy_Turret_Class = BP_Enemy_Turret_Class.Class;

}

TSubclassOf<AActor> UEnemySpawnLocation::GetCurrentTypeClass() const
{
    switch (SpawnType)
    {
    case Enemy_Charger:
      return Enemy_Charger_Class;
      break;
    case Enemy_Shooter:
      return Enemy_Shooter_Class;
      break;
    case Enemy_Turret:
      return Enemy_Turret_Class;
      break;
    default:
      return TSubclassOf<AActor>();
      break;
    }
}

FVector UEnemySpawnLocation::GetBoxCenter() const
{
    if (!ensure(BoxComponent != nullptr)) { return FVector(0); }

    return BoxComponent->Bounds.GetBox().GetCenter();
}

bool UEnemySpawnLocation::HasEnemyInBounds()
{
    return bEnemyInBounds;
}

void UEnemySpawnLocation::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor)
    {
        if (OtherActor->IsA<ASuper_AI_Character>())
        {
            bEnemyInBounds = true;
        }
    }
}

void UEnemySpawnLocation::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		if (OtherActor->IsA<ASuper_AI_Character>())
		{
			bEnemyInBounds = false;
		}
	}
}

