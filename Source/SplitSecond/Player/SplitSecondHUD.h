// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SplitSecondHUD.generated.h"

UCLASS()
class ASplitSecondHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASplitSecondHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	void ToggleDebugMenu();

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
	TSubclassOf<class UDebugMenu> DebugMenuClass;
	class UDebugMenu* DebugMenu = nullptr;
};

