// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SplitSecondGameMode.h"
#include "Player/SplitSecondHUD.h"
#include "Player/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/WorldSettings.h"
#include "Player/SplitSecondPlayerController.h"

ASplitSecondGameMode::ASplitSecondGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprint/Player/BP_PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASplitSecondHUD::StaticClass();
}

void ASplitSecondGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldSettings()->MaxGlobalTimeDilation = MaxTimeDilation;
	GetWorldSettings()->MinGlobalTimeDilation = MinTimeDilation;
}

void ASplitSecondGameMode::SetDefaultWeapon(EWeapons NewWeapon)
{
	auto PC = GetWorld()->GetFirstPlayerController<ASplitSecondPlayerController>();
	if (!ensure(PC != nullptr)) { return; }
	PC->SetDefaultWeapon(NewWeapon);
}