// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SplitSecondGameMode.h"
#include "Player/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/WorldSettings.h"
#include "Player/SplitSecondPlayerController.h"
#include "Weapons/Super_Gun.h"
#include "SplitSecondPlayerState.h"
#include "World/Arena.h"
#include "SplitSecondGameState.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/UpgradeSelection.h"
#include "AI/Super_AI_Character.h"
#include "Weapons/SplitSecondProjectile.h"
#include "Weapons/Guns/AI/AIProjectile.h"
#include "World/Traps/SuperTrap.h"
#include "UI/PlayerUI.h"
#include "SplitSecondGameInstance.h"
#include "Health/HealthComponent.h"
#include "UI/TransitionScreen.h"

ASplitSecondGameMode::ASplitSecondGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprint/Player/BP_PlayerCharacter"));
	if (PlayerPawnClassFinder.Class)DefaultPawnClass = PlayerPawnClassFinder.Class;
	static ConstructorHelpers::FClassFinder<ASuper_Gun> Pistol_BP(TEXT("/Game/Blueprint/Player/Weapons/BP_PlayerPistol"));
	if(Pistol_BP.Class) PistolClass = Pistol_BP.Class;
	static ConstructorHelpers::FClassFinder<ASuper_Gun> Shotgun_BP(TEXT("/Game/Blueprint/Player/Weapons/BP_PlayerShotgun"));
	if (Shotgun_BP.Class) ShotgunClass = Shotgun_BP.Class;
	static ConstructorHelpers::FClassFinder<ASuper_Gun> Bow_BP(TEXT("/Game/Blueprint/Player/Weapons/BP_PlayerBow"));
	if (Bow_BP.Class) BowClass = Bow_BP.Class;

	ConstructorHelpers::FClassFinder<UPlayerUI> BP_PlayerUIClass(TEXT("/Game/Blueprint/UI/WBP_PlayerUI"));
	if (BP_PlayerUIClass.Class) PlayerUIClass = BP_PlayerUIClass.Class;

	static ConstructorHelpers::FClassFinder<UUpgradeSelection> BP_UpgradeSelectionClass(TEXT("/Game/Blueprint/UI/WBP_UpgradeSelection"));
	if (BP_UpgradeSelectionClass.Class) UpgradeSelectionClass = BP_UpgradeSelectionClass.Class;

	static ConstructorHelpers::FClassFinder<UTransitionScreen> BP_TransitionScreenClass(TEXT("/Game/Blueprint/UI/WBP_TransitionScreen"));
	if (BP_TransitionScreenClass.Class) TransitionScreenClass = BP_TransitionScreenClass.Class;
}

void ASplitSecondGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldSettings()->MaxGlobalTimeDilation = MaxTimeDilation;
	GetWorldSettings()->MinGlobalTimeDilation = MinTimeDilation;

	SpawnNextArena();
}
void ASplitSecondGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer->IsA<ASplitSecondPlayerController>())
	{
		SplitSecondPlayerController = Cast<ASplitSecondPlayerController>(NewPlayer);

		if (!ensure(SplitSecondPlayerController != nullptr)) { return; }

		SplitSecondPlayerController->OnPlayerDeath.BindUFunction(this, TEXT("OnPlayerDeath"));
		SplitSecondPlayerController->OnPlayerConfirmedDeath.BindUFunction(this, TEXT("OnConfirmedPlayerDeath"));
		SplitSecondPlayerController->OnPlayerSlowGame.BindUFunction(this, TEXT("PlayerSlowGame"));

		SplitSecondPlayerState = SplitSecondPlayerController->GetPlayerState<ASplitSecondPlayerState>();
		if (!ensure(SplitSecondPlayerState != nullptr)) { return; }

		SplitSecondPlayerCharacter = SplitSecondPlayerController->GetPawn<APlayerCharacter>();
		if (!ensure(SplitSecondPlayerCharacter != nullptr)) { return; }

		auto GameInstance = GetGameInstance<USplitSecondGameInstance>();
		if (!ensure(GameInstance != nullptr)) { return; }
		PlayerGun = GameInstance->GetStartingWeapon();

		auto Pawn = SplitSecondPlayerController->GetPawn<APlayerCharacter>();
		if (!ensure(Pawn != nullptr)) { return; }
		Pawn->SpawnPlayerUI(PlayerUIClass);
		SetPlayerDefaultWeapon(PlayerGun, Pawn);
	}

}
void ASplitSecondGameMode::SetPlayerDefaultWeapon(EWeapons NewWeapon, APlayerCharacter* PlayerPawn)
{
	if (!ensure(SplitSecondPlayerController != nullptr)) { return; }
	if (!ensure(PlayerPawn != nullptr)) { return; }
	if (!ensure(SplitSecondPlayerState != nullptr)) { return; }

	switch (NewWeapon)
	{
	case Pistol:
		SplitSecondPlayerState->CurrentStats = FUpgrades(DefaultStatsForPistol);
		PlayerPawn->EquipGun(PistolClass);
		break;
	case Shotgun:
		SplitSecondPlayerState->CurrentStats = FUpgrades(DefaultStatsForShotgun);
		PlayerPawn->EquipGun(ShotgunClass);
		break;
	case Bow:
		SplitSecondPlayerState->CurrentStats = FUpgrades(DefaultStatsForBow);
		PlayerPawn->EquipGun(BowClass);
		break;
	default:
		break;
	}
	
}

void ASplitSecondGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckSlowGame();
}
void ASplitSecondGameMode::CheckSlowGame()
{
	if (bGameIsSlowed && GetWorld()->GetTimeSeconds() >= SlowEndTime)
	{
		for (auto SlowedActor : SlowedActors)
		{
			if (SlowedActor)
			{
				if (auto SlowedEnemy = Cast<ASuper_AI_Character>(SlowedActor))
				{
					if (SlowedEnemy->GetIsSlowed())
					{
						continue;
					}
				}
				SlowedActor->CustomTimeDilation = 1;
			}
		}
		SlowedActors.Empty();
		bGameIsSlowed = false;
	}
}

void ASplitSecondGameMode::SpawnNextArena()
{
	++ArenaNum;
	UE_LOG(LogTemp, Log, TEXT("Arena Num: %i"), ArenaNum);

	if (CurrentArena)
	{
		CurrentArena->Destroy();
		CurrentArena = nullptr;
	}

	if (!ensure(PossibleArenas.Num() > 0)) { return; }

	if (ArenaNum % 10 != 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawning Normal Arena"));

		auto RoomIndex = FMath::RandRange(0, PossibleArenas.Num() - 1);
		CurrentArena = GetWorld()->SpawnActor<AArena>(PossibleArenas[RoomIndex]);
		if (!ensure(CurrentArena != nullptr)) { return; }
		CurrentArena->OnArenaFinished.BindUFunction(this, TEXT("HandleArenaFinished"));
		CurrentArena->SpawnActors();
		UNavigationSystemV1::GetNavigationSystem(GetWorld())->Build();

		if (!ArenaMusicReference)
		{
			if (!ensure(ArenaMusic != nullptr)) { return; }
			ArenaMusicReference = UGameplayStatics::SpawnSound2D(GetWorld(), ArenaMusic);
		}
		else
		{
			ArenaMusicReference->SetActive(true);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Spawning Boss Arena"));

		CurrentArena = GetWorld()->SpawnActor<AArena>(BossArena);
		if (!ensure(CurrentArena != nullptr)) { return; }
		CurrentArena->OnArenaFinished.BindUFunction(this, TEXT("HandleBossFinished"));
		CurrentArena->SpawnActors();
		UNavigationSystemV1::GetNavigationSystem(GetWorld())->Build();

		if (!BossMusicReference)
		{
			if (!ensure(BossMusic != nullptr)) { return; }
			BossMusicReference = UGameplayStatics::SpawnSound2D(GetWorld(), BossMusic);
		}
		else
		{
			BossMusicReference->SetActive(true);
		}
	}
}
void ASplitSecondGameMode::HandleArenaFinished()
{
	if (auto Created = CreateWidget<UTransitionScreen>(GetWorld(), TransitionScreenClass))
	{
		Created->OnTransitionFinished.BindUFunction(this, TEXT("SpawnUpgradeScreen"));
		Created->PlayWinAnimation(ArenaNum);
	}
}
void ASplitSecondGameMode::HandleBossFinished()
{
	if (auto Created = CreateWidget<UTransitionScreen>(GetWorld(), TransitionScreenClass))
	{
		Created->OnTransitionFinished.BindUFunction(this, TEXT("SpawnBossUpgradeScreen"));
		Created->PlayWinAnimation(ArenaNum);
	}
}

void ASplitSecondGameMode::SpawnUpgradeScreen()
{
	if (!ensure(SplitSecondPlayerState != nullptr)) { return; }

	if (auto Created = CreateWidget<UUpgradeSelection>(GetWorld(), UpgradeSelectionClass))
	{
		Created->ShowUpgradeSelection(&SplitSecondPlayerState->CurrentStats, PlayerGun, false);
		Created->OnUpgradeSelected.BindUFunction(this, TEXT("SpawnNextArena"));
	}

	;
}
void ASplitSecondGameMode::SpawnBossUpgradeScreen()
{
	if (!ensure(SplitSecondPlayerState != nullptr)) { return; }

	if (auto Created = CreateWidget<UUpgradeSelection>(GetWorld(), UpgradeSelectionClass))
	{
		Created->ShowUpgradeSelection(&SplitSecondPlayerState->CurrentStats, PlayerGun, true);
		Created->OnUpgradeSelected.BindUFunction(this, TEXT("SpawnNextArena"));
	}

	SplitSecondPlayerState->CurrentStats.Health += SplitSecondPlayerState->CurrentStats.MaxHealth;
}
void ASplitSecondGameMode::HandleEnemyDeath()
{
	++Kills;
}

void ASplitSecondGameMode::PlayerSlowGame()
{
	if (!ensure(SplitSecondPlayerState != nullptr)) { return; }

	bGameIsSlowed = true;
	CurrentSlowValue = SplitSecondPlayerState->CurrentStats.GameSlowValue;

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASuper_AI_Character::StaticClass(), FoundActors);

	for (auto FoundActor : FoundActors)
	{
		if (auto SlowedEnemy = Cast<ASuper_AI_Character>(FoundActor))
		{
			if (SlowedEnemy->GetIsSlowed())
			{
				continue;
			}
		}
		FoundActor->CustomTimeDilation = CurrentSlowValue;
		SlowedActors.Add(FoundActor);
	}

	TArray<AActor*> FoundProjectiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIProjectile::StaticClass(), FoundProjectiles);

	for (auto FoundProjectile : FoundProjectiles)
	{
		FoundProjectile->CustomTimeDilation = CurrentSlowValue;
		SlowedActors.Add(FoundProjectile);
	}

	TArray<AActor*> FoundTraps;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASuperTrap::StaticClass(), FoundTraps);

	for (auto FoundTrap : FoundTraps)
	{
		FoundTrap->CustomTimeDilation = CurrentSlowValue;
		SlowedActors.Add(FoundTrap);
	}

	SlowEndTime = GetWorld()->GetTimeSeconds() + SplitSecondPlayerState->CurrentStats.GameSlowDuration;
}
void ASplitSecondGameMode::AddActorToSlowedArray(AActor* ActorToSlow)
{
	if (!ensure(ActorToSlow != nullptr)) { return; }

	if (bGameIsSlowed)
	{
		SlowedActors.Add(ActorToSlow);
		ActorToSlow->CustomTimeDilation = CurrentSlowValue;
	}
}

void ASplitSecondGameMode::OnPlayerDeath()
{
	///This triggers immediately after the player dies
	if (auto Created = CreateWidget<UTransitionScreen>(GetWorld(), TransitionScreenClass))
	{
		Created->OnTransitionFinished.BindUFunction(this, TEXT("OnConfirmedPlayerDeath"));
		Created->PlayFailAnimation(ArenaNum);
	}

}
void ASplitSecondGameMode::OnConfirmedPlayerDeath()
{
	///This triggers after the player decides to confirm his death by hitting a button

	if (!ensure(SplitSecondPlayerController != nullptr)) { return; }
	SplitSecondPlayerController->ClientTravel(FString("/Game/Maps/MainMenu"), ETravelType::TRAVEL_Absolute, false);
}