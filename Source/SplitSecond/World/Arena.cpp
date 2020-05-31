// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "Arena.h"
#include "EnemySpawnLocation.h"
#include "Engine/World.h"
#include "TrapPlacer.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerController.h"
#include "Flag.h"
#include "FlagTarget.h"
#include "../AI/Super_Boss.h"
#include "../World/Traps/SuperTrap.h"
#include "TargetLocation.h"
#include "../AI/Super_AI_Character.h"
#include "../UI/PopupMessage.h"
#include "../AI/AI_TurretBase.h"
#include "../Weapons/Guns/AI/AIProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "BossSpawnLocation.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "../Player/PlayerCharacter.h"
#include "../UI/PlayerUI.h"
#include "../SplitSecondGameMode.h"

// Sets default values
AArena::AArena()
{
	PrimaryActorTick.bCanEverTick = true;

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
void AArena::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckSpawnEnemyWaveTimer();
	CheckSurviveTimer();
}
void AArena::CheckSpawnEnemyWaveTimer()
{
	if (bSpawningEnemyWaves && GetWorld()->GetTimeSeconds() >= WaveTimerTargetTime)
	{
		if (!ensure(CurrentSettings != nullptr)) { return; }

		if (CurrentObjective != EObjectives::KillAllEnemies || CurrentObjective != EObjectives::KillBoss)
		{
			if (!ensure(PlayerPawn != nullptr)) { return; }
			PlayerPawn->GetPlayerUI()->SetNextWaveTime(CurrentSettings->WaveInterval);
		}
		
		TArray<UEnemySpawnLocation*> EnemySpawnLocations;
		GetComponents<UEnemySpawnLocation>(EnemySpawnLocations);
		if (!ensure(EnemySpawnLocations.Num() >= CurrentSettings->EnemiesPerWaveMin)) { return; }


		auto SpawnNum = FMath::RandRange(CurrentSettings->EnemiesPerWaveMin, CurrentSettings->EnemiesPerWaveMax);
		SpawnEnemies(SpawnNum, EnemySpawnLocations);

		WaveTimerTargetTime = GetWorld()->GetTimeSeconds() + CurrentSettings->WaveInterval;
	}
}
void AArena::CheckSurviveTimer()
{
	if (bSurviving && GetWorld()->GetTimeSeconds() >= SurviveTargetTime)
	{
		FinishObjective();
	}
}
void AArena::SpawnActors()
{
	if (!ensure(Objectives.Num() > 0)) { return; }

	if (!ensure(GetWorld() != nullptr)) { return; }
	if (!ensure(GetWorld()->GetFirstPlayerController() != nullptr)) { return; }
	if (!ensure(GetWorld()->GetFirstPlayerController()->GetPawn() != nullptr)) { return; }

	PlayerPawn = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (!ensure(PlayerPawn != nullptr)) { return; }

	PlayerPawn->SetActorLocation(PlayerStartLocation->GetComponentLocation());
	GetWorld()->GetFirstPlayerController()->SetControlRotation(PlayerStartLocation->GetComponentRotation());

	TArray<TEnumAsByte<EObjectives>> OutKeys;
	Objectives.GetKeys(OutKeys);
	CurrentObjective = OutKeys[FMath::RandRange(0, OutKeys.Num() - 1)];

	CurrentSettings = Objectives.Find(CurrentObjective);

	if (auto CreatedWidget = CreateWidget<UPopupMessage>(GetWorld(), PopupMessageClass))
	{
		if (CurrentSettings)
		{
			switch (CurrentObjective)
			{
			case Survive:
				CreatedWidget->ShowPopupMessage(FKey("F"), FText::FromString("survive\n\npress f to start"));
				CreatedWidget->OnConditionFufilled.BindUFunction(this, TEXT("SetupSurvive"));
				break;
			case ReachObjective:
				CreatedWidget->ShowPopupMessage(FKey("F"), FText::FromString("reach the end\n\npress f to start"));
				CreatedWidget->OnConditionFufilled.BindUFunction(this, TEXT("SetupObjective"));
				break;
			case CaptureTheFlag:
				CreatedWidget->ShowPopupMessage(FKey("F"), FText::FromString("capture the tablet\n\npress f to start"));
				CreatedWidget->OnConditionFufilled.BindUFunction(this, TEXT("SetupFlag"));
				break;
			case KillAllEnemies:
				CreatedWidget->ShowPopupMessage(FKey("F"), FText::FromString("kill all enemy waves\n\npress f to start"));
				CreatedWidget->OnConditionFufilled.BindUFunction(this, TEXT("SetupKillAll"));
				break;
			case KillBoss:
				CreatedWidget->ShowPopupMessage(FKey("F"), FText::FromString("kill sir epoch\n\npress f to start"));
				CreatedWidget->OnConditionFufilled.BindUFunction(this, TEXT("SetupKillBoss"));
			case Tutorial:
				CreatedWidget->ShowPopupMessage(FKey("F"), FText::FromString("Learn game\n\npress f to start"));
				CreatedWidget->OnConditionFufilled.BindUFunction(this, TEXT("SetupTutorial"));
			default:
				break;
			}
		}
	}
}
void AArena::SetupSurvive()
{
	bSpawningEnemyWaves = true;
	WaveTimerTargetTime = GetWorld()->GetTimeSeconds();

	bSurviving = true;
	SurviveTargetTime = GetWorld()->GetTimeSeconds() + CurrentSettings->SurviveTime;

	PlayerPawn->GetPlayerUI()->SetSurviveTime(CurrentSettings->SurviveTime);
}
void AArena::SetupFlag()
{
	bSpawningEnemyWaves = true;
	WaveTimerTargetTime = GetWorld()->GetTimeSeconds();

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
void AArena::SetupTutorial()
{
	if (auto Spawned = GetWorld()->SpawnActor<ATargetLocation>(LocationTargetScene->GetComponentLocation(), FRotator(0)))
	{
		Spawned->OnTargetLocationCollision.BindUFunction(this, FName("FinishObjective"));
		Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}
void AArena::TryDeliverFlag()
{
	if (bHasFlag)
	{
		UE_LOG(LogTemp, Log, TEXT("Flag Delivered"));
		FinishObjective();
		if (SpawnedFlagTarget)
		{
			SpawnedFlagTarget->PlayFinishSound();
			SpawnedFlagTarget->Destroy();
		}
	}
}
void AArena::SetupObjective()
{
	bSpawningEnemyWaves = true;
	WaveTimerTargetTime = GetWorld()->GetTimeSeconds();

	if (auto Spawned = GetWorld()->SpawnActor<ATargetLocation>(LocationTargetScene->GetComponentLocation(), FRotator(0)))
	{
		Spawned->OnTargetLocationCollision.BindUFunction(this, FName("FinishObjective"));
		Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}

	UE_LOG(LogTemp, Log, TEXT("Reach Objective Setup Finished"));
}
void AArena::SetupKillAll()
{
	SpawnKillAllWave();
	PlayerPawn->GetPlayerUI()->SetRemainingEnemies(CurrentSettings->EnemyTotal);
}
void AArena::SpawnKillAllWave()
{
	if (!ensure(CurrentSettings != nullptr)) { return; }
	
	if (CurrentSettings->EnemyTotal <= 0) FinishObjective();
	if (CurrentSettings->EnemyTotal < CurrentSettings->EnemiesPerWaveMax)
	{
		CurrentSettings->EnemiesPerWaveMax = CurrentSettings->EnemyTotal;
		if (CurrentSettings->EnemiesPerWaveMin > CurrentSettings->EnemiesPerWaveMax)
		{
			CurrentSettings->EnemiesPerWaveMin = CurrentSettings->EnemiesPerWaveMax;
		}
	}

	TArray<UEnemySpawnLocation*> EnemySpawnLocations;
	GetComponents<UEnemySpawnLocation>(EnemySpawnLocations);
	if (!ensure(EnemySpawnLocations.Num() >= CurrentSettings->EnemiesPerWaveMin)) { return; }

	auto SpawnNum = FMath::RandRange(CurrentSettings->EnemiesPerWaveMin, CurrentSettings->EnemiesPerWaveMax);
	SpawnEnemies(SpawnNum, EnemySpawnLocations);
}
void AArena::SetupKillBoss()
{
	if (auto SpawnLocation = GetComponentByClass(UBossSpawnLocation::StaticClass()))
	{
		auto BossSpawnLocationComponent = Cast<UBossSpawnLocation>(SpawnLocation);
		if (auto Spawned = GetWorld()->SpawnActor<ASuper_Boss>(BossSpawnLocationComponent->GetCurrentTypeClass(), FTransform(FRotator(0), BossSpawnLocationComponent->GetBoxCenter(), FVector(1))))
		{
			Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			Spawned->OnDestroyed.AddUniqueDynamic(this, &AArena::OnBossDeath);
			SpawnedEnemies.Add(Spawned);
		}
	}
}

void AArena::SpawnEnemies(int32 SpawnNum, TArray<UEnemySpawnLocation*> SpawnLocations)
{
	TArray<int32> SpawnedIndexes;
	for (size_t i = 0; i < SpawnNum; i++)
	{
		for (size_t j = 0; j < MAX_ATTEMPTS; j++)
		{
			int32 SpawnIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
			if (!SpawnedIndexes.Contains(SpawnIndex))
			{
				SpawnedIndexes.Add(SpawnIndex);
				break;
			}
		}
	}

	for (size_t i = 0; i < SpawnedIndexes.Num(); i++)
	{
		auto EnemySpawnLocationComponent = Cast<UEnemySpawnLocation>(SpawnLocations[SpawnedIndexes[i]]);
		if (!SpawnedTurrets.Find(EnemySpawnLocationComponent))
		{
			FTransform SpawnTranform = FTransform(FRotator(0), EnemySpawnLocationComponent->GetBoxCenter(), FVector(1));
			if (auto Spawned = GetWorld()->SpawnActor<ASuper_AI_Character>(EnemySpawnLocationComponent->GetCurrentTypeClass(), SpawnTranform))
			{
				Spawned->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				Spawned->OnDestroyed.AddUniqueDynamic(this, &AArena::HandleEnemyDeath);
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

void AArena::FinishObjective()
{
	bSurviving = false;
	bSpawningEnemyWaves = false;

	ObjectiveFinished = true;

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
			EnemyToDestroy->Destroy();
		}
	}
	SpawnedEnemies.Empty();

	FinishArena();

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIProjectile::StaticClass(), OutActors);
	for (auto Projectile : OutActors)
	{
		Projectile->Destroy();
	}

	UE_LOG(LogTemp, Log, TEXT("Objective Finished"));

	if (!ensure(PlayerPawn != nullptr)) { return; }
	PlayerPawn->GetPlayerUI()->HandleArenaFinished();
}
void AArena::FinishArena()
{
	UE_LOG(LogTemp, Log, TEXT("Arena Finished"));
	OnArenaFinished.ExecuteIfBound();
}

void AArena::HandleEnemyDeath(AActor* KilledAI)
{
	if (ObjectiveFinished) return;
	if (!ensure(CurrentSettings != nullptr)) { return; }

	auto GameMode = GetWorld()->GetAuthGameMode<ASplitSecondGameMode>();
	if (!ensure(GameMode != nullptr)) { return; }
	GameMode->HandleEnemyDeath();

	SpawnedEnemies.Remove(KilledAI);

	if (CurrentObjective == EObjectives::KillAllEnemies)
	{
		if (!ensure(PlayerPawn != nullptr)) { return; }
		--CurrentSettings->EnemyTotal;
		PlayerPawn->GetPlayerUI()->SetRemainingEnemies(CurrentSettings->EnemyTotal);
	}

	if (SpawnedEnemies.Num() <= 0)
	{
		if (CurrentObjective == EObjectives::KillAllEnemies)
		{
			SpawnKillAllWave();
		}
	}
}

void AArena::OnBossDeath(AActor* KilledAI)
{
	FinishObjective();
}

void AArena::OnTurretDeath(AActor* KilledAI)
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
