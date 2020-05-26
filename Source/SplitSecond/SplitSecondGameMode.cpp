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

ASplitSecondGameMode::ASplitSecondGameMode()
	: Super()
{
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

void ASplitSecondGameMode::SpawnNextArena()
{
	if (CurrentArena) CurrentArena->Destroy(); 

	auto MyGameState = Cast<ASplitSecondGameState>(GameState);
	if (!ensure(MyGameState != nullptr)) { return; }

	if (!ensure(PossibleArenas.Num() > 0)) { return; }

	if (ArenaNum % 10 != 0 || ArenaNum == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("Spawning Normal Arena"));

		auto RoomIndex = FMath::RandRange(0, PossibleArenas.Num() - 1);
		CurrentArena = GetWorld()->SpawnActor<AArena>(PossibleArenas[RoomIndex]);
		if (!ensure(CurrentArena != nullptr)) { return; }
		CurrentArena->OnArenaFinished.BindUFunction(this, TEXT("SpawnUpgradeScreen"));
		CurrentArena->OnEnemyDeath.BindUFunction(this, TEXT("HandleEnemyDeath"));
		CurrentArena->SpawnActors();
		UNavigationSystemV1::GetNavigationSystem(GetWorld())->Build();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Spawning Boss Arena"));

		CurrentArena = GetWorld()->SpawnActor<AArena>(BossArena);
		if (!ensure(CurrentArena != nullptr)) { return; }
		CurrentArena->OnArenaFinished.BindUFunction(this, TEXT("SpawnBossUpgradeScreen"));
		CurrentArena->SpawnActors();
		UNavigationSystemV1::GetNavigationSystem(GetWorld())->Build();
	}
	UE_LOG(LogTemp, Log, TEXT("Arena Num: %i"), ArenaNum);
	++ArenaNum;
}
void ASplitSecondGameMode::HandleEnemyDeath()
{
	++Kills;
}
void ASplitSecondGameMode::SpawnUpgradeScreen()
{
	UE_LOG(LogTemp, Log, TEXT("Showing Arena Upgrade Screen"), ArenaNum);

	if (!ensure(SplitSecondPlayerState != nullptr)) { return; }
	if (!ensure(SplitSecondPlayerCharacter != nullptr)) { return; }
	if (auto Created = CreateWidget<UUpgradeSelection>(GetWorld(), UpgradeSelectionClass))
	{
		Created->ShowUpgradeSelection(&SplitSecondPlayerState->CurrentStats, PlayerGun, false, SplitSecondPlayerCharacter->GetHealthComponent());
		Created->OnUpgradeSelected.BindUFunction(this, TEXT("SpawnNextArena"));
	}

	if (SplitSecondPlayerCharacter)
	{
		SplitSecondPlayerCharacter->GetHealthComponent()->Heal(SplitSecondPlayerCharacter->GetHealthComponent()->GetMaxHealth() * 0.1);
	}
}
void ASplitSecondGameMode::SpawnBossUpgradeScreen()
{
	UE_LOG(LogTemp, Log, TEXT("Showing Boss Upgrade Screen"), ArenaNum);

	if (!ensure(SplitSecondPlayerState != nullptr)) { return; }
	if (!ensure(SplitSecondPlayerCharacter != nullptr)) { return; }

	if (auto Created = CreateWidget<UUpgradeSelection>(GetWorld(), UpgradeSelectionClass))
	{
		Created->ShowUpgradeSelection(&SplitSecondPlayerState->CurrentStats, PlayerGun, true, SplitSecondPlayerCharacter->GetHealthComponent());
		Created->OnUpgradeSelected.BindUFunction(this, TEXT("SpawnNextArena"));
	}

	if (SplitSecondPlayerCharacter)
	{
		SplitSecondPlayerCharacter->GetHealthComponent()->Heal(SplitSecondPlayerCharacter->GetHealthComponent()->GetMaxHealth() * 0.1);
	}
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

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ASplitSecondGameMode::StopPlayerSlowGame, SplitSecondPlayerState->CurrentStats.GameSlowDuration, false);
	SlowEndTime = SplitSecondPlayerState->CurrentStats.GameSlowDuration + GetWorld()->GetTimeSeconds();
}

void ASplitSecondGameMode::StopPlayerSlowGame()
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
	
	bGameIsSlowed = false;
}

void ASplitSecondGameMode::OnPlayerDeath()
{
	///This triggers immediately after the player dies

}
void ASplitSecondGameMode::OnConfirmedPlayerDeath()
{
	///This triggers after the player decides to confirm his death by hitting a button

	//TODO Move player back to the main menu / Restart the game
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}