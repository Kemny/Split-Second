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
#include "Kismet/KismetSystemLibrary.h"

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
	auto MyGameState = Cast<ASplitSecondGameState>(GameState);
	if (!ensure(MyGameState != nullptr)) { return; }
	UE_LOG(LogTemp, Log, TEXT("Current Level: %i"), MyGameState->GetCurrentLevel());

	if (!ensure(PossibleArenas.Num() > 0)) { return; }

	if (MyGameState->GetCurrentLevel() % 10 != 0)
	{
		auto RoomIndex = FMath::RandRange(0, PossibleArenas.Num() - 1);
		if (auto Spawned = GetWorld()->SpawnActor<AArena>(PossibleArenas[RoomIndex]))
		{
			PossibleArenas.RemoveAt(RoomIndex);
			Spawned->OnArenaFinished.BindUFunction(this, TEXT("SpawnNextArena"));
			Spawned->SpawnActors();
			UNavigationSystemV1::GetNavigationSystem(GetWorld())->Build();
		}
	}
	else
	{
		switch (MyGameState->GetCurrentLevel())
		{
		case 10:
			UE_LOG(LogTemp, Log, TEXT("Spawning Boss Room #1"));
			break;
		case 20:
			UE_LOG(LogTemp, Log, TEXT("Spawning Boss Room #2"));
			break;
		case 30:
			UE_LOG(LogTemp, Log, TEXT("Spawning Boss Room #3"));
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("Something went wrong when spawning a level"));
			break;
		}
	}
}

void ASplitSecondGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer->IsA<ASplitSecondPlayerController>())
	{
		auto PC = Cast<ASplitSecondPlayerController>(NewPlayer);

		PC->OnPlayerDeath.BindUFunction(this, TEXT("OnPlayerDeath"));
		PC->OnPlayerConfirmedDeath.BindUFunction(this, TEXT("OnConfirmedPlayerDeath"));
	}

}

void ASplitSecondGameMode::OnPlayerDeath()
{
	///This triggers immediately after the player dies

	//TODO Move player back to the main menu / Restart the game
}
void ASplitSecondGameMode::OnConfirmedPlayerDeath()
{
	///This triggers after the player decides to confim his death by hitting a button

	//TODO Move player back to the main menu / Restart the game
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
}