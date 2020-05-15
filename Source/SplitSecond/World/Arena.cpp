// This project falls under CC-BY-SA lisence

#include "Arena.h"
#include "EnemySpawnLocation.h"
#include "RandomEnemySpawner.h"
#include "ActorSpawnLocationComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AArena::AArena()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArena::BeginPlay()
{
	Super::BeginPlay();
	
}

void AArena::SpawnActors()
{
	auto SpawnLocations = GetComponentsByClass(UActorSpawnLocationComponent::StaticClass());
	for (auto ActorToSpawn : SpawnLocations)
	{
		if (auto ActorSpawnLocationComponent = Cast<UActorSpawnLocationComponent>(ActorToSpawn))
		{
			if (ActorSpawnLocationComponent->SpawnType == Player_Location)
			{
				GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(ActorSpawnLocationComponent->GetComponentLocation());
			}
			else
			{
				TSubclassOf<AActor> ActorClassToSpawn = ActorSpawnLocationComponent->GetCurrentTypeClass();
				if (auto Spawned = GetWorld()->SpawnActor<AActor>(ActorClassToSpawn))
				{
					Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					Spawned->SetActorLocation(ActorSpawnLocationComponent->GetComponentLocation());
				}
			}
		}
	}
	auto EnemySpawnLocations = GetComponentsByClass(UEnemySpawnLocation::StaticClass());
	for (auto ActorToSpawn : EnemySpawnLocations)
	{
		if (auto EnemySpawnLocationComponent = Cast<UEnemySpawnLocation>(ActorToSpawn))
		{
			TSubclassOf<AActor> EnemyClassToSpawn = EnemySpawnLocationComponent->GetCurrentTypeClass();
			if (auto Spawned = GetWorld()->SpawnActor<AActor>(EnemyClassToSpawn))
			{
				Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				Spawned->SetActorLocation(EnemySpawnLocationComponent->GetBoxCenter());
			}
		}
	}
	auto RandomEnemySpawnLocations = GetComponentsByClass(URandomEnemySpawner::StaticClass());
	for (auto ActorToSpawn : RandomEnemySpawnLocations)
	{
		if (auto RandomEnemySpawnLocationComponent = Cast<URandomEnemySpawner>(ActorToSpawn))
		{
			RandomEnemySpawnLocationComponent->SpawnEnemies(this);
		}
	}
}

void AArena::FinishObjective()
{
	//TODO Spawn a button or something to let the player progress
}

void AArena::FinishArena()
{
	///TODO after the condition created from finish objective is fulled, we call finished arena to tell the gamemode to spawn the next one
	OnArenaFinished.ExecuteIfBound();
	Destroy();
}

