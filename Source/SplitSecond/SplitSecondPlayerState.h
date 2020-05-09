// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SplitSecondPlayerState.generated.h"

USTRUCT()
struct FUpgrades
{
    GENERATED_USTRUCT_BODY()

public:
    bool bIsPiercing;
    bool bIsHoming;
    bool bIsBouncing;
    bool bHasDoubleBullets;
    bool bExplodingBullets;
    bool bBossOneHat;
    bool bBossTwoHat;
    bool bBoosThreeHat;

    bool bHasExtraLife;
    bool bReverseCubeSlowing;
    bool bHasShield;
    bool bBiggerBullets;
    bool bCanThrowGun;
    bool bConstantDashSpeed;

    float LifeStealPercentage;

};

UCLASS()
class SPLITSECOND_API ASplitSecondPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
    FUpgrades CurrentUpgrades;

};
