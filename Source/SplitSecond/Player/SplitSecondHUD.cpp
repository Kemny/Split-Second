// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SplitSecondHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "../UI/DebugMenu.h"

ASplitSecondHUD::ASplitSecondHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	static ConstructorHelpers::FClassFinder<UDebugMenu> BP_DebugMenuClass(TEXT("/Game/Blueprint/UI/WBP_DebugMenu"));
	if (BP_DebugMenuClass.Class) DebugMenuClass = BP_DebugMenuClass.Class;

}

void ASplitSecondHUD::DrawHUD()
{
	Super::DrawHUD();

}

void ASplitSecondHUD::ToggleDebugMenu()
{
	if (!DebugMenu)
	{
		DebugMenu = CreateWidget<UDebugMenu>(GetOwningPlayerController(), DebugMenuClass);
		DebugMenu->AddToPlayerScreen();
	}
	else
	{
		DebugMenu->RemoveFromParent();
		DebugMenu = nullptr;
	}
}