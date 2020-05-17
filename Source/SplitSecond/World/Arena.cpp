// This project falls under CC-BY-SA lisence

#include "Arena.h"
#include "EnemySpawnLocation.h"
#include "RandomEnemySpawner.h"
#include "ActorSpawnLocationComponent.h"
#include "Engine/World.h"
#include "TrapPlacer.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerController.h"
#include "Flag.h"
#include "../World/Traps/SuperTrap.h"
#include "TargetLocation.h"
#include "../AI/Super_AI_Character.h"
#include "../UI/PopupMessage.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"

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

void AArena::SpawnActors(const FArenaSettings& NewSettings)
{
	if (!ensure(PossibleObjectives.Num() > 0)) { return; }
	CurrentSettings = NewSettings;

	auto SpawnLocations = GetComponentsByClass(UActorSpawnLocationComponent::StaticClass());
	TArray<UActorSpawnLocationComponent*> ActorSpawnLocationComponents;

	for (auto ActorToSpawn : SpawnLocations)
	{
		if (auto ActorSpawnLocationComponent = Cast<UActorSpawnLocationComponent>(ActorToSpawn))
		{
			ActorSpawnLocationComponents.Add(ActorSpawnLocationComponent);
			if (ActorSpawnLocationComponent->SpawnType == Player_Location)
			{
				GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(ActorSpawnLocationComponent->GetComponentLocation());
				ActorSpawnLocationComponent->DestroyComponent(false);
			}
		}
	}

	switch (PossibleObjectives[FMath::RandRange(0, PossibleObjectives.Num() - 1)])
	{
	case Survive:
		///Spawn Enemies until a timer runs out
		SetupSurvive();
		break;
	case ReachObjective:
		///Spawn a PlayerTargetLocation && Survive enemies
		SetupObjective(ActorSpawnLocationComponents);
		break;
	case CaptureTheFlag:
		///Spawn A Flag and FlagTarget && Survive enemies
		SetupFlag(ActorSpawnLocationComponents);
		break;
	case KillAllEnemies:
		///Spawn a few waves of enemies, each one after the previous dies.
		SetupKillAll();
		break;
	default:
		break;
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
				Spawned->SetActorRotation(TrapSpawnLocationComponent->GetRelativeRotation());
			}
		}
	}
}

void AArena::SetupSurvive()
{
	GetWorldTimerManager().SetTimer(SurviveHandle, this, &AArena::FinishSurvive, CurrentSettings.SurvivalSettings.SurviveTime, false);
	GetWorldTimerManager().SetTimer(SpawnEnemiesHandle, this, &AArena::SpawnNextEnemyWave_Survival, CurrentSettings.SurvivalSettings.WaveInterval, true, 0);
}
void AArena::SpawnNextEnemyWave_Survival()
{
	auto EnemySpawnLocations = GetComponentsByClass(UEnemySpawnLocation::StaticClass());
	if (!ensure(EnemySpawnLocations.Num() >= CurrentSettings.SurvivalSettings.EnemiesPerWaveMin)) { return; }

	auto SpawnNum = FMath::RandRange(CurrentSettings.SurvivalSettings.EnemiesPerWaveMin, CurrentSettings.SurvivalSettings.EnemiesPerWaveMax);

	SpawnEnemies(SpawnNum, EnemySpawnLocations);
}
void AArena::FinishSurvive()
{
	FinishObjective();
}

void AArena::SetupFlag(TArray<UActorSpawnLocationComponent*> SpawnLocations)
{
	GetWorldTimerManager().SetTimer(SpawnEnemiesHandle, this, &AArena::SpawnNextEnemyWave_CaptureTheFlag, CurrentSettings.SurvivalSettings.WaveInterval, true, 0);

	///Spawn Flag
	for (auto ActorSpawnLocationComponent : SpawnLocations)
	{
		switch (ActorSpawnLocationComponent->SpawnType)
		{
		case Objective_Flag:
			if (auto Spawned = GetWorld()->SpawnActor<AFlag>(ActorSpawnLocationComponent->Objective_Flag_Class))
			{
				Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				Spawned->SetActorLocation(ActorSpawnLocationComponent->GetComponentLocation());
				Spawned->OnFlagCollision.BindUFunction(this, FName("AquireFlag"));
				ActorSpawnLocationComponent->DestroyComponent(false);
			}
			break;
		case Objective_FlagTarget:
			if (auto Spawned = GetWorld()->SpawnActor<AFlag>(ActorSpawnLocationComponent->Objective_FlagTarget_Class))
			{
				Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				Spawned->SetActorLocation(ActorSpawnLocationComponent->GetComponentLocation());
				Spawned->OnFlagCollision.BindUFunction(this, FName("TryDeliverFlag"));
				ActorSpawnLocationComponent->DestroyComponent(false);
			}
		default:
			break;
		}
	}
}
void AArena::SpawnNextEnemyWave_CaptureTheFlag()
{
	auto EnemySpawnLocations = GetComponentsByClass(UEnemySpawnLocation::StaticClass());
	if (!ensure(EnemySpawnLocations.Num() >= CurrentSettings.CaptureTheFlagSettings.EnemiesPerWaveMin)) { return; }

	auto SpawnNum = FMath::RandRange(CurrentSettings.CaptureTheFlagSettings.EnemiesPerWaveMin, CurrentSettings.CaptureTheFlagSettings.EnemiesPerWaveMax);

	SpawnEnemies(SpawnNum, EnemySpawnLocations);
}
void AArena::TryDeliverFlag()
{
	if (bHasFlag)
	{
		FinishObjective();
	}
}

void AArena::SetupObjective(TArray<UActorSpawnLocationComponent*> SpawnLocations)
{
	GetWorldTimerManager().SetTimer(SpawnEnemiesHandle, this, &AArena::SpawnNextEnemyWave_ReachTheObjective, CurrentSettings.ReachObjectiveSettings.WaveInterval, true, 0);

	///Spawn Flag
	for (auto ActorSpawnLocationComponent : SpawnLocations)
	{
		switch (ActorSpawnLocationComponent->SpawnType)
		{
		case Objective_PlayerTarget:
			if (auto Spawned = GetWorld()->SpawnActor<ATargetLocation>(ActorSpawnLocationComponent->Objective_PlayerTarget_Class))
			{
				Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				Spawned->SetActorLocation(ActorSpawnLocationComponent->GetComponentLocation());
				Spawned->OnTargetLocationCollision.BindUFunction(this, FName("FinishObjective"));
				ActorSpawnLocationComponent->DestroyComponent(false);
			}
			break;
		default:
			break;
		}
	}
}
void AArena::SpawnNextEnemyWave_ReachTheObjective()
{
	auto EnemySpawnLocations = GetComponentsByClass(UEnemySpawnLocation::StaticClass());
	if (!ensure(EnemySpawnLocations.Num() >= CurrentSettings.ReachObjectiveSettings.EnemiesPerWaveMin)) { return; }

	auto SpawnNum = FMath::RandRange(CurrentSettings.ReachObjectiveSettings.EnemiesPerWaveMin, CurrentSettings.ReachObjectiveSettings.EnemiesPerWaveMax);

	SpawnEnemies(SpawnNum, EnemySpawnLocations);
}

void AArena::SetupKillAll()
{
	///TODO This cannot be implemented before we have enemy death state
}

void AArena::SpawnEnemies(int32 SpawnNum, TArray<UActorComponent*> SpawnLocations)
{
	TArray<int32> SpawnedIndexes;
	for (size_t i = 0; i < SpawnNum; i++)
	{
		int32 SpawnIndex;
		do
		{
			SpawnIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
		} while (SpawnedIndexes.Contains(SpawnIndex));

		SpawnedIndexes.Add(SpawnIndex);
	}

	for (size_t i = 0; i < SpawnedIndexes.Num(); i++)
	{
		auto ActorToSpawn = SpawnLocations[SpawnedIndexes[i]];
		if (auto EnemySpawnLocationComponent = Cast<UEnemySpawnLocation>(ActorToSpawn))
		{
			if (auto Spawned = GetWorld()->SpawnActor<AActor>(EnemySpawnLocationComponent->GetCurrentTypeClass()))
			{
				Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				Spawned->SetActorLocation(EnemySpawnLocationComponent->GetBoxCenter());
			}
		}
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

	// Destroy Level
	TArray<USceneComponent*> ChildActors;
	FDetachmentTransformRules DetachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	
	GetComponents<USceneComponent>(ChildActors);

	// For what ever reason this doesn't destroy enemies or traps not sure why
	for (auto Actor : ChildActors)
	{
		Actor->DetachFromParent(false, false);
		Actor->DestroyComponent();
	}

	// Destroy Traps
	TArray<AActor*> FoundTraps;
	UGameplayStatics::GetAllActorsOfClass(this, ASuperTrap::StaticClass(), FoundTraps);

	for (auto Trap : FoundTraps)
	{
		if (Trap)
		{
			Trap->Destroy();
		}
	}

	// Destroy Enemies
	TArray<AActor*> FoundEnemis;
	UGameplayStatics::GetAllActorsOfClass(this, ASuper_AI_Character::StaticClass(), FoundTraps);

	for (auto Enemy : FoundEnemis)
	{
		if (Enemy)
		{
			Enemy->Destroy();
		}
	}


	Destroy();
}


