// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SplitSecondGameMode.h"
#include "Player/SplitSecondHUD.h"
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

	static ConstructorHelpers::FClassFinder<UUpgradeSelection> BP_UpgradeSelectionClass(TEXT("/Game/Blueprint/UI/WBP_UpgradeSelection"));
	if (BP_UpgradeSelectionClass.Class) UpgradeSelectionClass = BP_UpgradeSelectionClass.Class;

	// use our custom HUD class
	HUDClass = ASplitSecondHUD::StaticClass();
}

void ASplitSecondGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldSettings()->MaxGlobalTimeDilation = MaxTimeDilation;
	GetWorldSettings()->MinGlobalTimeDilation = MinTimeDilation;

	switch (Temp_StartingGun)
	{
	case Pistol:
		SetDefaultWeapon(Temp_StartingGun, PistolClass);
		break;
	case Shotgun:
		SetDefaultWeapon(Temp_StartingGun, ShotgunClass);
		break;
	case Bow:
		SetDefaultWeapon(Temp_StartingGun, BowClass);
		break;
	default:
		break;
	}

	StartPlay();

	SpawnNextArena();
}

void ASplitSecondGameMode::SetDefaultWeapon(EWeapons NewWeapon, TSubclassOf<ASuper_Gun> WeaponClass)
{
	auto PC = GetWorld()->GetFirstPlayerController<ASplitSecondPlayerController>();
	if (!ensure(PC != nullptr)) { return; }
	auto Pawn = Cast<APlayerCharacter>(PC->K2_GetPawn());
	if (!ensure(Pawn != nullptr)) { return; }
	Pawn->EquipGun(WeaponClass);
	auto PS = Pawn->GetPlayerState<ASplitSecondPlayerState>();
	if (!ensure(PC != nullptr)) { return; }

	switch (NewWeapon)
	{
	case Pistol:
		PS->CurrentStats = FUpgrades(DefaultStatsForPistol);
		break;
	case Shotgun:
		PS->CurrentStats = FUpgrades(DefaultStatsForShotgun);
		break;
	case Bow:
		PS->CurrentStats = FUpgrades(DefaultStatsForBow);
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

	if (ArenaNum % 10 != 0)
	{
		auto RoomIndex = FMath::RandRange(0, PossibleArenas.Num() - 1);
		CurrentArena = GetWorld()->SpawnActor<AArena>(PossibleArenas[RoomIndex]);
		if (CurrentArena)
		{
			CurrentArena->OnArenaFinished.BindUFunction(this, TEXT("SpawnUpgradeScreen"));
			CurrentArena->SpawnActors();
			UNavigationSystemV1::GetNavigationSystem(GetWorld())->Build();
		}
	}
	else
	{
		CurrentArena = GetWorld()->SpawnActor<AArena>(BossArena);
		if(CurrentArena)
		{
			CurrentArena->OnArenaFinished.BindUFunction(this, TEXT("SpawnBossUpgradeScreen"));
			CurrentArena->SpawnActors();
			UNavigationSystemV1::GetNavigationSystem(GetWorld())->Build();
		}
	}
	++ArenaNum;
}

void ASplitSecondGameMode::SpawnUpgradeScreen()
{
	if (!ensure(SplitSecondPlayerState != nullptr)) { return; }
	if (!ensure(SplitSecondPlayerCharacter != nullptr)) { return; }
	if (auto Created = CreateWidget<UUpgradeSelection>(GetWorld(), UpgradeSelectionClass))
	{
		Created->ShowUpgradeSelection(&SplitSecondPlayerState->CurrentStats, Temp_StartingGun, false, SplitSecondPlayerCharacter->GetHealthComponent());
		Created->OnUpgradeSelected.BindUFunction(this, TEXT("SpawnNextArena"));
	}
}
void ASplitSecondGameMode::SpawnBossUpgradeScreen()
{
	if (!ensure(SplitSecondPlayerState != nullptr)) { return; }
	if (!ensure(SplitSecondPlayerCharacter != nullptr)) { return; }

	if (auto Created = CreateWidget<UUpgradeSelection>(GetWorld(), UpgradeSelectionClass))
	{
		Created->ShowUpgradeSelection(&SplitSecondPlayerState->CurrentStats, Temp_StartingGun, true, SplitSecondPlayerCharacter->GetHealthComponent());
		Created->OnUpgradeSelected.BindUFunction(this, TEXT("SpawnNextArena"));
	}
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
	}

}

void ASplitSecondGameMode::PlayerSlowGame()
{
	if (!ensure(SplitSecondPlayerState != nullptr)) { return; }

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASuper_AI_Character::StaticClass(), FoundActors);

	for (auto FoundActor : FoundActors)
	{
		FoundActor->CustomTimeDilation = SplitSecondPlayerState->CurrentStats.GameSlowValue;
		SlowedActors.Add(FoundActor);
	}

	TArray<AActor*> FoundProjectiles;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIProjectile::StaticClass(), FoundProjectiles);

	for (auto FoundProjectile : FoundProjectiles)
	{
		FoundProjectile->CustomTimeDilation = SplitSecondPlayerState->CurrentStats.GameSlowValue;
		SlowedActors.Add(FoundProjectile);
	}

	TArray<AActor*> FoundTraps;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASuperTrap::StaticClass(), FoundTraps);

	for (auto FoundTrap : FoundTraps)
	{
		FoundTrap->CustomTimeDilation = SplitSecondPlayerState->CurrentStats.GameSlowValue;
		SlowedActors.Add(FoundTrap);
	}

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ASplitSecondGameMode::StopPlayerSlowGame, SplitSecondPlayerState->CurrentStats.GameSlowDuration, false);
}

void ASplitSecondGameMode::StopPlayerSlowGame()
{
	for (auto SlowedActor : SlowedActors)
	{
		if (SlowedActor)
		{
			SlowedActor->CustomTimeDilation = 1;
		}
	}
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