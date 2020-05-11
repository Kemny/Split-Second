// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SplitSecondGameMode.h"
#include "Player/SplitSecondHUD.h"
#include "Player/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/WorldSettings.h"
#include "Player/SplitSecondPlayerController.h"
#include "Weapons/Super_Gun.h"

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

}

void ASplitSecondGameMode::SetDefaultWeapon(EWeapons NewWeapon, TSubclassOf<ASuper_Gun> Pistol)
{
	auto PC = GetWorld()->GetFirstPlayerController<ASplitSecondPlayerController>();
	if (!ensure(PC != nullptr)) { return; }
	PC->SetDefaultWeapon(NewWeapon, Pistol);
}