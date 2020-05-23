// This project falls under CC-BY-SA lisence

#include "Arena.h"
#include "EnemySpawnLocation.h"
#include "Engine/World.h"
#include "TrapPlacer.h"
#include "Engine/EngineTypes.h"
#include "BossAddsSpawnLocation.h"
#include "GameFramework/PlayerController.h"
#include "Flag.h"
#include "FlagTarget.h"
#include "../AI/Super_Boss.h"
#include "../World/Traps/SuperTrap.h"
#include "TargetLocation.h"
#include "../AI/Super_AI_Character.h"
#include "../UI/PopupMessage.h"
#include "../AI/AI_TurretBase.h"
#include "BossSpawnLocation.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "../Player/PlayerCharacter.h"
#include "../UI/PlayerUI.h"

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

	FlagScene = CreateDefaultSubobject<USceneComponent>(TEXT("Flag"));
	FlagScene->SetupAttachment(Root);
	FlagScene->SetHiddenInGame(true);
	
	FlagTargetScene = CreateDefaultSubobject<USceneComponent>(TEXT("FlagTarget"));
	FlagTargetScene->SetupAttachment(Root);
	FlagTargetScene->SetHiddenInGame(true);

	LocationTargetScene = CreateDefaultSubobject<USceneComponent>(TEXT("LocationTarget"));
	LocationTargetScene->SetupAttachment(Root);
	LocationTargetScene->SetHiddenInGame(true);
}

void AArena::SpawnActors()
{
	if (!ensure(Objectives.Num() > 0)) { return; }

	if (!ensure(GetWorld() != nullptr)) { return; }
	if (!ensure(GetWorld()->GetFirstPlayerController() != nullptr)) { return; }
	if (!ensure(GetWorld()->GetFirstPlayerController()->GetPawn() != nullptr)) { return; }

	auto PlayerPawn = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!ensure(PlayerPawn != nullptr)) { return; }
	PlayerPawn->SetActorLocation(PlayerStartLocation->GetComponentLocation());
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
			PlayerPawn->GetPlayerUI()->SetObjectiveName(FString("Survive"));
			break;
		case ReachObjective:
			GetWorldTimerManager().SetTimer(SpawnEnemiesHandle, this, &AArena::SpawnNextEnemyWave, Settings->WaveInterval, true, 0);
			PlayerPawn->GetPlayerUI()->SetObjectiveName(FString("Reach the End"));
			SetupObjective();
			break;
		case CaptureTheFlag:
			GetWorldTimerManager().SetTimer(SpawnEnemiesHandle, this, &AArena::SpawnNextEnemyWave, Settings->WaveInterval, true, 0);
			PlayerPawn->GetPlayerUI()->SetObjectiveName(FString("Capture The Flag"));
			SetupFlag();
			break;
		case KillAllEnemies:
			SetupKillAll();
			PlayerPawn->GetPlayerUI()->SetObjectiveName(FString("Kill All Enemies"));
			break;
		default:
			break;
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
	if (auto Spawned = GetWorld()->SpawnActor<AFlag>(FlagScene->GetComponentLocation(), FRotator(0)))
	{
		Spawned->OnFlagCollision.BindUFunction(this, FName("AquireFlag"));
		Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
	SpawnedFlagTarget = GetWorld()->SpawnActor<AFlagTarget>(FlagTargetScene->GetComponentLocation(), FRotator(0));
	if (!ensure(SpawnedFlagTarget != nullptr)) { return; }
	SpawnedFlagTarget->OnFlagTargetCollision.BindUFunction(this, FName("TryDeliverFlag"));
	SpawnedFlagTarget->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	UE_LOG(LogTemp, Log, TEXT("Capture The Flag Setup Finished"));
}
void AArena::TryDeliverFlag()
{
	if (bHasFlag)
	{
		UE_LOG(LogTemp, Log, TEXT("Flag Delivered"));
		FinishObjective();
		if (SpawnedFlagTarget)
		{
			SpawnedFlagTarget->Destroy();
		}
	}
}
void AArena::SetupObjective()
{
	if (auto Spawned = GetWorld()->SpawnActor<ATargetLocation>(LocationTargetScene->GetComponentLocation(), FRotator(0)))
	{
		Spawned->OnTargetLocationCollision.BindUFunction(this, FName("FinishObjective"));
		Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}

	UE_LOG(LogTemp, Log, TEXT("Reach Objective Setup Finished"));
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
		if (!SpawnedTurrets.Find(EnemySpawnLocationComponent))
		{
			if (auto Spawned = GetWorld()->SpawnActor<ASuper_AI_Character>(EnemySpawnLocationComponent->GetCurrentTypeClass(), FTransform(FRotator(0), EnemySpawnLocationComponent->GetBoxCenter(), FVector(1))))
			{
				Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				Spawned->OnDeath.AddUniqueDynamic(this, &AArena::OnEnemyDeath);
				SpawnedEnemies.Add(Spawned);
				if (Spawned->IsA<AAI_TurretBase>())
				{
					Spawned->OnDestroyed.AddUniqueDynamic(this, &AArena::OnTurretDeath);
					SpawnedTurrets.Add(EnemySpawnLocationComponent, Spawned);
				}
			}
		}
	}
}

void AArena::SpawnBoss(int32 SpawnNum, TArray<UActorComponent*> SpawnLocations)
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
		auto BossSpawnLocationComponent = Cast<UBossSpawnLocation>(SpawnLocations[SpawnedIndexes[i]]);
		if (auto Spawned = GetWorld()->SpawnActor<ASuper_Boss>(BossSpawnLocationComponent->GetCurrentTypeClass(), FTransform(FRotator(0), BossSpawnLocationComponent->GetBoxCenter(), FVector(1))))
		{
			Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Spawned->OnDeath.AddUniqueDynamic(this, &AArena::OnEnemyDeath);
			SpawnedEnemies.Add(Spawned);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Spawn Boss"));
}

void AArena::SpawnBossAdds(int32 SpawnNum, TArray<UActorComponent*> SpawnLocations)
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
		auto BossAddsSpawnLocationComponent = Cast<UBossAddsSpawnLocation>(SpawnLocations[SpawnedIndexes[i]]);
		if (auto Spawned = GetWorld()->SpawnActor<ASuper_AI_Character>(BossAddsSpawnLocationComponent->GetCurrentTypeClass(), FTransform(FRotator(0), BossAddsSpawnLocationComponent->GetBoxCenter(), FVector(1))))
		{
			Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Spawned->OnDeath.AddUniqueDynamic(this, &AArena::OnEnemyDeath);
			SpawnedEnemies.Add(Spawned);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Spawn Boss Adds"));
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
	for (auto EnemyToDestroy : SpawnedEnemies)
	{
		if (EnemyToDestroy)
		{
			EnemyToDestroy->DetachFromActor(DetachRules);
			EnemyToDestroy->DetachFromControllerPendingDestroy();
			EnemyToDestroy->Destroy();
		}
	}

	///Prompt player into entering next level
	if (auto Spawned = CreateWidget<UPopupMessage>(GetWorld(), PopupMessageClass))
	{
		Spawned->ShowPopupMessage(FKey("F"), FText::FromString("PRESS F TO CONTINUE"));
		Spawned->OnConditionFufilled.BindUFunction(this, TEXT("FinishArena"));
	}

	UE_LOG(LogTemp, Log, TEXT("Objective Finished"));
}
void AArena::FinishArena()
{
	UE_LOG(LogTemp, Log, TEXT("Arena Finished"));
	OnArenaFinished.ExecuteIfBound();
}

void AArena::OnEnemyDeath(ASuper_AI_Character* KilledAI)
{
	SpawnedEnemies.Remove(KilledAI);
}

void AArena::OnTurretDeath(ASuper_AI_Character* KilledAI)
{
	TArray<UEnemySpawnLocation*> OutKeys;
	SpawnedTurrets.GetKeys(OutKeys);
	for (auto Key : OutKeys)
	{
		if (auto FoundValue = *SpawnedTurrets.Find(Key))
		{
			if (FoundValue == KilledAI)
			{
				SpawnedTurrets.Remove(Key);
				FoundValue->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
				FoundValue->Destroy();
			}
		}
	}
}
