// This project falls under CC-BY-SA lisence

#include "Arena.h"
#include "EnemySpawnLocation.h"
#include "RandomEnemySpawner.h"
#include "ActorSpawnLocationComponent.h"
#include "Engine/World.h"
#include "TrapPlacer.h"
#include "GameFramework/PlayerController.h"
#include "Flag.h"
#include "TargetLocation.h"
#include "../UI/PopupMessage.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AArena::AArena()
{
	PrimaryActorTick.bCanEverTick = false;

	ConstructorHelpers::FClassFinder<UPopupMessage> BP_PopupMessageClass(TEXT("/Game/Blueprint/UI/WBP_PopupMessage"));
	if (BP_PopupMessageClass.Class) PopupMessageClass = BP_PopupMessageClass.Class;

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
			switch (ActorSpawnLocationComponent->SpawnType)
			{
			case Player_Location:
				GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(ActorSpawnLocationComponent->GetComponentLocation());
				break;

			case Objective_Flag:
				if (auto Spawned = GetWorld()->SpawnActor<AFlag>(ActorSpawnLocationComponent->Objective_Flag_Class))
				{
					Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					Spawned->SetActorLocation(ActorSpawnLocationComponent->GetComponentLocation());
					Spawned->OnFlagCollision.BindUFunction(this, FName("AquireFlag"));
				}
				break;

			case Objective_FlagTarget:
				if (auto Spawned = GetWorld()->SpawnActor<AFlag>(ActorSpawnLocationComponent->Objective_FlagTarget_Class))
				{
					Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					Spawned->SetActorLocation(ActorSpawnLocationComponent->GetComponentLocation());
					Spawned->OnFlagCollision.BindUFunction(this, FName("TryDeliverFlag"));
				}
				break;

			case Objective_PlayerTarget:
				if (auto Spawned = GetWorld()->SpawnActor<ATargetLocation>(ActorSpawnLocationComponent->Objective_PlayerTarget_Class))
				{
					Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					Spawned->SetActorLocation(ActorSpawnLocationComponent->GetComponentLocation());
					Spawned->OnTargetLocationCollision.BindUFunction(this, FName("FinishObjective"));
				}
				break;
			default:
				break;
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
	auto TrapPlacersLocations = GetComponentsByClass(UTrapPlacer::StaticClass());
	for (auto ActorToSpawn : TrapPlacersLocations)
	{
		if (auto TrapSpawnLocationComponent = Cast<UTrapPlacer>(ActorToSpawn))
		{
			TSubclassOf<AActor> TrapClassToSpawn = TrapSpawnLocationComponent->GetCurrentTypeClass();
			if (auto Spawned = GetWorld()->SpawnActor<AActor>(TrapClassToSpawn))
			{
				Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				Spawned->SetActorLocation(TrapSpawnLocationComponent->GetBoxCenter());
			}
		}
	}
}

void AArena::TryDeliverFlag()
{
	if (bHasFlag)
	{
		FinishObjective();
	}
}

void AArena::FinishObjective()
{
	if (auto Spawned = CreateWidget<UPopupMessage>(GetWorld(), PopupMessageClass))
	{
		Spawned->AddToPlayerScreen();
		Spawned->SetUserFocus(GetWorld()->GetFirstPlayerController());
		Spawned->ShowPopupMessage(FKey("F"), FText::FromString("PRESS F TO CONTINUE"));
		Spawned->OnConditionFufilled.BindUFunction(this, TEXT("FinishArena"));
	}
}

void AArena::FinishArena()
{
	OnArenaFinished.ExecuteIfBound();
	Destroy();
}

