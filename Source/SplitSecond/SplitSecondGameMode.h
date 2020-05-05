// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SplitSecondGameMode.generated.h"

UCLASS(minimalapi)
class ASplitSecondGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)float MinTimeDilation = 0.01;
	UPROPERTY(EditAnywhere)float MaxTimeDilation = 1;

public:
	ASplitSecondGameMode();

	void BeginPlay() override;
};



