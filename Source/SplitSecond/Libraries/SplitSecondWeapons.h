// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SplitSecondWeapons.generated.h"

UENUM()
enum EWeapons
{
    Pistol,
    Shotgun,
    Bow
};

USTRUCT(BlueprintType)
struct FPistolDefault
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere)bool bIsPiercing;
    UPROPERTY(EditAnywhere)bool bIsHoming;
    UPROPERTY(EditAnywhere)bool bIsBouncing;
    UPROPERTY(EditAnywhere)bool bHasDoubleBullets;
    UPROPERTY(EditAnywhere)bool bExplodingBullets;

    UPROPERTY(EditAnywhere)bool bHasShield;
    UPROPERTY(EditAnywhere)bool bBiggerBullets;
    UPROPERTY(EditAnywhere)bool bCanThrowGun;
    UPROPERTY(EditAnywhere)bool bConstantDashSpeed;

    UPROPERTY(EditAnywhere)float LifeStealPercentage;

    UPROPERTY(EditAnywhere)float Ammo;
    UPROPERTY(EditAnywhere)float Damgage;
    UPROPERTY(EditAnywhere)float ProjectileSpeed;
    UPROPERTY(EditAnywhere)float ReloadSpeed;
};
USTRUCT(BlueprintType)
struct FShotgunDefault
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere)bool bIsPiercing;
    UPROPERTY(EditAnywhere)bool bIsHoming;
    UPROPERTY(EditAnywhere)bool bIsBouncing;
    UPROPERTY(EditAnywhere)bool bHasDoubleBullets;
    UPROPERTY(EditAnywhere)bool bExplodingBullets;

    UPROPERTY(EditAnywhere)bool bHasShield;
    UPROPERTY(EditAnywhere)bool bBiggerBullets;
    UPROPERTY(EditAnywhere)bool bCanThrowGun;
    UPROPERTY(EditAnywhere)bool bConstantDashSpeed;

    UPROPERTY(EditAnywhere)float LifeStealPercentage;

    UPROPERTY(EditAnywhere)float Ammo;
    UPROPERTY(EditAnywhere)float Damgage;
    UPROPERTY(EditAnywhere)float ProjectileSpeed;
    UPROPERTY(EditAnywhere)float ReloadSpeed;
};
USTRUCT(BlueprintType)
struct FBowDefault
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere)bool bIsPiercing;
    UPROPERTY(EditAnywhere)bool bIsHoming;
    UPROPERTY(EditAnywhere)bool bIsBouncing;
    UPROPERTY(EditAnywhere)bool bHasDoubleBullets;
    UPROPERTY(EditAnywhere)bool bExplodingBullets;

    UPROPERTY(EditAnywhere)bool bHasShield;
    UPROPERTY(EditAnywhere)bool bBiggerBullets;
    UPROPERTY(EditAnywhere)bool bCanThrowGun;
    UPROPERTY(EditAnywhere)bool bConstantDashSpeed;

    UPROPERTY(EditAnywhere)float LifeStealPercentage;

    UPROPERTY(EditAnywhere)float Ammo;
    UPROPERTY(EditAnywhere)float Damgage;
    UPROPERTY(EditAnywhere)float ProjectileSpeed;
    UPROPERTY(EditAnywhere)float ReloadSpeed;
};

USTRUCT(BlueprintType)
struct FUpgrades
{
    GENERATED_USTRUCT_BODY()

public:
    UPROPERTY(EditAnywhere)bool bIsPiercing;
    UPROPERTY(EditAnywhere)bool bIsHoming;
    UPROPERTY(EditAnywhere)bool bIsBouncing;
    UPROPERTY(EditAnywhere)bool bHasDoubleBullets;
    UPROPERTY(EditAnywhere)bool bExplodingBullets;
    UPROPERTY(EditAnywhere)bool bBossOneHat;
    UPROPERTY(EditAnywhere)bool bBossTwoHat;
    UPROPERTY(EditAnywhere)bool bBoosThreeHat;

    UPROPERTY(EditAnywhere)bool bHasExtraLife;
    UPROPERTY(EditAnywhere)bool bReverseCubeSlowing;
    UPROPERTY(EditAnywhere)bool bHasShield;
    UPROPERTY(EditAnywhere)bool bBiggerBullets;
    UPROPERTY(EditAnywhere)bool bCanThrowGun;
    UPROPERTY(EditAnywhere)bool bConstantDashSpeed;

    UPROPERTY(EditAnywhere)float LifeStealPercentage;

    UPROPERTY(EditAnywhere)float Ammo;
    UPROPERTY(EditAnywhere)float Damgage;
    UPROPERTY(EditAnywhere)float ProjectileSpeed;
    UPROPERTY(EditAnywhere)float ReloadSpeed;

    UPROPERTY(EditAnywhere)float ProjectileSpeedMultiplier = 1;
    UPROPERTY(EditAnywhere)float DamageMultiplier = 1;
    UPROPERTY(EditAnywhere) float ReloadSpeedMultiplier = 1;

    FUpgrades()
    {

    }
    FUpgrades(FPistolDefault Defaults)
    {
        bIsPiercing = Defaults.bIsPiercing;
        bIsHoming = Defaults.bIsHoming;
        bIsBouncing = Defaults.bIsBouncing;
        bHasDoubleBullets = Defaults.bHasDoubleBullets;
        bExplodingBullets = Defaults.bExplodingBullets;

        bHasShield = Defaults.bHasShield;
        bBiggerBullets = Defaults.bBiggerBullets;
        bCanThrowGun = Defaults.bCanThrowGun;
        bConstantDashSpeed = Defaults.bConstantDashSpeed;

        LifeStealPercentage = Defaults.LifeStealPercentage;

        Ammo = Defaults.Ammo;
        Damgage = Defaults.Damgage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        ReloadSpeed = Defaults.ReloadSpeed;
    }
    FUpgrades(FShotgunDefault Defaults)
    {
        bIsPiercing = Defaults.bIsPiercing;
        bIsHoming = Defaults.bIsHoming;
        bIsBouncing = Defaults.bIsBouncing;
        bHasDoubleBullets = Defaults.bHasDoubleBullets;
        bExplodingBullets = Defaults.bExplodingBullets;

        bHasShield = Defaults.bHasShield;
        bBiggerBullets = Defaults.bBiggerBullets;
        bCanThrowGun = Defaults.bCanThrowGun;
        bConstantDashSpeed = Defaults.bConstantDashSpeed;

        LifeStealPercentage = Defaults.LifeStealPercentage;

        Ammo = Defaults.Ammo;
        Damgage = Defaults.Damgage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        ReloadSpeed = Defaults.ReloadSpeed;
    }
    FUpgrades(FBowDefault Defaults)
    {
        bIsPiercing = Defaults.bIsPiercing;
        bIsHoming = Defaults.bIsHoming;
        bIsBouncing = Defaults.bIsBouncing;
        bHasDoubleBullets = Defaults.bHasDoubleBullets;
        bExplodingBullets = Defaults.bExplodingBullets;

        bHasShield = Defaults.bHasShield;
        bBiggerBullets = Defaults.bBiggerBullets;
        bCanThrowGun = Defaults.bCanThrowGun;
        bConstantDashSpeed = Defaults.bConstantDashSpeed;

        LifeStealPercentage = Defaults.LifeStealPercentage;

        Ammo = Defaults.Ammo;
        Damgage = Defaults.Damgage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        ReloadSpeed = Defaults.ReloadSpeed;
    }
};

UCLASS()
class SPLITSECOND_API USplitSecondWeapons : public UObject
{
	GENERATED_BODY()
	
};
