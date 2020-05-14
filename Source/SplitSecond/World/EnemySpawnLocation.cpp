// This project falls under CC-BY-SA lisence


#include "EnemySpawnLocation.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values for this component's properties
UEnemySpawnLocation::UEnemySpawnLocation()
{
    SetVisibility(false);

    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    BoxComponent->SetupAttachment(this);
    BoxComponent->SetBoxExtent(FVector(32, 32, 95));
    
    ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Charger_Class(TEXT("/Game/Blueprint/NPC/BP_Charger"));
    if (BP_Enemy_Charger_Class.Class) Enemy_Charger_Class = BP_Enemy_Charger_Class.Class;

    ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Healer_Class(TEXT("/Game/Blueprint/NPC/BP_Healer"));
    if (BP_Enemy_Healer_Class.Class) Enemy_Healer_Class = BP_Enemy_Healer_Class.Class;

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
    case Enemy_Healer:
      return Enemy_Healer_Class;
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
