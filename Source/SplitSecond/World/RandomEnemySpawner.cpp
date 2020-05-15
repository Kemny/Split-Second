// This project falls under CC-BY-SA lisence


#include "RandomEnemySpawner.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values for this component's properties
URandomEnemySpawner::URandomEnemySpawner()
{
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
    BoxComponent->SetupAttachment(this);

    ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Charger_Class(TEXT("/Game/Blueprint/NPC/BP_Charger"));
    if (BP_Enemy_Charger_Class.Class) Enemy_Charger_Class = BP_Enemy_Charger_Class.Class;

    ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Shooter_Class(TEXT("/Game/Blueprint/NPC/BP_Shooter"));
    if (BP_Enemy_Shooter_Class.Class) Enemy_Shooter_Class = BP_Enemy_Shooter_Class.Class;

    BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    BoxComponent->SetBoxExtent(FVector(1500, 4000, 100));

    AmountToSpawn = 5;
}

void URandomEnemySpawner::SpawnEnemies(AActor* Parent)
{
	if (!ensure(Parent != nullptr)) { return; }

	for (int32 Index = 0; Index < AmountToSpawn; Index++)
	{
		if (auto RandomEnemy = GetRandomType())
		{
			FVector RandomLocation = GetRandomPoint();
			FTransform RandomTransform = FTransform(FRotator(0), RandomLocation, FVector(1));

			FActorSpawnParameters ActorParams;
			ActorParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			if (auto Spawned = GetWorld()->SpawnActor<AActor>(RandomEnemy, RandomTransform, ActorParams))
			{
				Spawned->AttachToActor(Parent, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
}

TSubclassOf<AActor> URandomEnemySpawner::GetRandomType() const
{
	ERandomSpawnEnemy EnemiesToSpawn = ERandomSpawnEnemy(FMath::Rand() % 2);

	switch (EnemiesToSpawn)
	{
	case E_Charger:
		return Enemy_Charger_Class;
		break;
	case E_Shooter:
		return Enemy_Shooter_Class;
		break;
	default:
		return TSubclassOf<AActor>();
		break;
	}
}

FVector URandomEnemySpawner::GetRandomPoint() const
{
  return UKismetMathLibrary::RandomPointInBoundingBox(GetOwner()->GetActorLocation(), BoxComponent->GetScaledBoxExtent());
}

