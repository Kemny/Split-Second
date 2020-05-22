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
	auto ChildActor = LocationTargetMesh->GetChildActor();
	if (!ensure(ChildActor != nullptr)) { return; }
	
	auto Objective = Cast<ATargetLocation>(ChildActor);
	if (!ensure(Objective != nullptr)) { return; }
	Objective->SetActorEnableCollision(true);
	Objective->OnTargetLocationCollision.BindUFunction(this, TEXT("FinishObjective"));
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
					Spawned->OnDeath.AddUniqueDynamic(this, &AArena::OnTurretDeath);
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
		EnemyToDestroy->DetachFromActor(DetachRules);
		EnemyToDestroy->DetachFromControllerPendingDestroy();
		EnemyToDestroy->Destroy();
	}

	///Prompt player into entering next level
	if (auto Spawned = CreateWidget<UPopupMessage>(GetWorld(), PopupMessageClass))
	{
		Spawned->ShowPopupMessage(FKey("F"), FText::FromString("PRESS F TO CONTINUE"));
		Spawned->OnConditionFufilled.BindUFunction(this, TEXT("FinishArena"));
	}
}
void AArena::FinishArena()
{
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
