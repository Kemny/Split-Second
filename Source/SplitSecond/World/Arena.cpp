// This project falls under CC-BY-SA lisence

#include "Arena.h"
#include "EnemySpawnLocation.h"
#include "RandomEnemySpawner.h"
#include "Engine/World.h"
#include "TrapPlacer.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerController.h"
#include "Flag.h"
#include "FlagTarget.h"
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

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	PlayerStartLocation = CreateDefaultSubobject<USceneComponent>(TEXT("Player Start Location"));
	PlayerStartLocation->SetupAttachment(Root);

	FlagMesh = CreateDefaultSubobject<UChildActorComponent>(TEXT("Flag"));
	FlagMesh->SetupAttachment(Root);
	FlagMesh->SetHiddenInGame(true);
	if (auto ChildActor = FlagMesh->GetChildActor()) ChildActor->SetActorEnableCollision(false);
	
	FlagTargetMesh = CreateDefaultSubobject<UChildActorComponent>(TEXT("FlagTarget"));
	FlagTargetMesh->SetupAttachment(Root);
	FlagTargetMesh->SetHiddenInGame(true);
	if (auto ChildActor = FlagTargetMesh->GetChildActor()) ChildActor->SetActorEnableCollision(false);

	LocationTargetMesh = CreateDefaultSubobject<UChildActorComponent>(TEXT("LocationTarget"));
	LocationTargetMesh->SetupAttachment(Root);
	LocationTargetMesh->SetHiddenInGame(true);
	if (auto ChildActor = LocationTargetMesh->GetChildActor()) ChildActor->SetActorEnableCollision(false);
}

void AArena::SpawnActors()
{
	if (!ensure(Objectives.Num() > 0)) { return; }

	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(PlayerStartLocation->GetComponentLocation());

	TArray<TEnumAsByte<EObjectives>> OutKeys;
	Objectives.GetKeys(OutKeys);
	CurrentObjective = OutKeys[FMath::RandRange(0, OutKeys.Num() - 1)];

	if (auto Settings = Objectives.Find(CurrentObjective))
	{
		switch (CurrentObjective)
		{
		case Survive:
			GetWorldTimerManager().SetTimer(SurviveHandle, this, &AArena::FinishObjective, Settings->SurviveTime, false);
			GetWorldTimerManager().SetTimer(SpawnEnemiesHandle, this, &AArena::SpawnNextEnemyWave, Settings->WaveInterval, true, 0);
			break;
		case ReachObjective:
			GetWorldTimerManager().SetTimer(SpawnEnemiesHandle, this, &AArena::SpawnNextEnemyWave, Settings->WaveInterval, true, 0);
			SetupObjective();
			break;
		case CaptureTheFlag:
			GetWorldTimerManager().SetTimer(SpawnEnemiesHandle, this, &AArena::SpawnNextEnemyWave, Settings->WaveInterval, true, 0);
			SetupFlag();
			break;
		case KillAllEnemies:
			SetupKillAll();
			break;
		default:
			break;
		}
		
	}
	auto TrapPlacersLocations = GetComponentsByClass(UTrapPlacer::StaticClass());
	for (auto ActorToSpawn : TrapPlacersLocations)
	{
		if (auto TrapSpawnLocationComponent = Cast<UTrapPlacer>(ActorToSpawn))
		{
			TSubclassOf<AActor> TrapClassToSpawn = TrapSpawnLocationComponent->TrapToPlace->GetChildActorClass();
			if (auto Spawned = GetWorld()->SpawnActor<AActor>(TrapClassToSpawn))
			{
				Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				Spawned->SetActorLocation(TrapSpawnLocationComponent->GetRelativeLocation());
				Spawned->SetActorRotation(TrapSpawnLocationComponent->GetRelativeRotation());
			}
		}
	}
}

void AArena::SpawnNextEnemyWave()
{
	auto Settings = Objectives.Find(CurrentObjective);
	if (!ensure(Settings != nullptr)) { return; }

	auto EnemySpawnLocations = GetComponentsByClass(UEnemySpawnLocation::StaticClass());
	if (!ensure(EnemySpawnLocations.Num() >= Settings->EnemiesPerWaveMin)) { return; }


	auto SpawnNum = FMath::RandRange(Settings->EnemiesPerWaveMin, Settings->EnemiesPerWaveMax);
	SpawnEnemies(SpawnNum, EnemySpawnLocations);
}
void AArena::SetupFlag()
{
	FlagMesh->SetHiddenInGame(false);
	if (auto ChildActor = FlagMesh->GetChildActor())
	{
		auto Objective = Cast<AFlag>(ChildActor);
		if (!ensure(Objective != nullptr)) { return; }
		Objective->SetActorEnableCollision(true);
		Objective->OnFlagCollision.BindUFunction(this, FName("AquireFlag"));
	}

	FlagTargetMesh->SetHiddenInGame(false);
	if (auto ChildActor = LocationTargetMesh->GetChildActor())
	{
		auto Objective = Cast<AFlagTarget>(ChildActor);
		if (!ensure(Objective != nullptr)) { return; }
		Objective->SetActorEnableCollision(true);
		Objective->OnFlagTargetCollision.BindUFunction(this, FName("TryDeliverFlag"));
	}
}
void AArena::TryDeliverFlag()
{
	if (bHasFlag)
	{
		FinishObjective();
	}
}
void AArena::SetupObjective()
{
	LocationTargetMesh->SetHiddenInGame(false);
	if (auto ChildActor = LocationTargetMesh->GetChildActor())
	{
		auto Objective = Cast<ATargetLocation>(ChildActor);
		if (!ensure(Objective != nullptr)) { return; }
		Objective->SetActorEnableCollision(true);
		Objective->OnTargetLocationCollision.BindUFunction(this, TEXT("FinishObjective"));
	}
}
void AArena::SetupKillAll()
{
	UE_LOG(LogTemp, Error, TEXT("Kill All Enemis Not Yet Implemented"));
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
		auto EnemySpawnLocationComponent = Cast<UEnemySpawnLocation>(SpawnLocations[SpawnedIndexes[i]]);
		if (auto Spawned = GetWorld()->SpawnActor<AActor>(EnemySpawnLocationComponent->GetCurrentTypeClass(), FTransform(FRotator(0), EnemySpawnLocationComponent->GetBoxCenter(), FVector(1))))
		{
			Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void AArena::FinishObjective()
{
	GetWorldTimerManager().ClearAllTimersForObject(this);
	FDetachmentTransformRules DetachRules = FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);

	// Destroy Traps
	TArray<AActor*> FoundTraps;
	UGameplayStatics::GetAllActorsOfClass(this, ASuperTrap::StaticClass(), FoundTraps);

	for (auto Trap : FoundTraps)
	{
		Trap->DetachFromActor(DetachRules);
		Trap->Destroy();
	}

	// Destroy Enemies
	TArray<AActor*> FoundEnemis;
	UGameplayStatics::GetAllActorsOfClass(this, ASuper_AI_Character::StaticClass(), FoundEnemis);

	for (auto Enemy : FoundEnemis)
	{
		if (auto EnemyToDestroy = Cast<ASuper_AI_Character>(Enemy))
		{
			EnemyToDestroy->DetachFromActor(DetachRules);
			EnemyToDestroy->DetachFromControllerPendingDestroy();
			EnemyToDestroy->Destroy();
		}
	}
	///Prompt player into entering next level
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
	TArray<USceneComponent*> ChildActors;
	GetComponents<USceneComponent>(ChildActors);
	for (auto Actor : ChildActors)
	{
		Actor->DetachFromParent(false, false);
		Actor->DestroyComponent();
	}

	OnArenaFinished.ExecuteIfBound();
	Destroy();
}


