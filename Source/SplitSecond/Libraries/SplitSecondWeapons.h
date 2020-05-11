// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SplitSecondWeapons.generated.h"

UENUM(Blueprintable)
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
    ///Bullets
    UPROPERTY(EditAnywhere)bool bIsPiercing;
    UPROPERTY(EditAnywhere)bool bIsHoming;
    UPROPERTY(EditAnywhere)bool bIsBouncing;
    UPROPERTY(EditAnywhere)bool bExplodingBullets;
    UPROPERTY(EditAnywhere)bool bBiggerBullets;

    ///Guns
    UPROPERTY(EditAnywhere)bool bHasShield;
    UPROPERTY(EditAnywhere)bool bCanThrowGun;
    UPROPERTY(EditAnywhere)bool bConstantDashSpeed;
    UPROPERTY(EditAnywhere)float Ammo;
    UPROPERTY(EditAnywhere)float MaxAmmo;
    UPROPERTY(EditAnywhere)float Damage;
    UPROPERTY(EditAnywhere)float ProjectileSpeed;
    UPROPERTY(EditAnywhere)float ReloadSpeed;
    UPROPERTY(EditAnywhere)float FireRate;
};
USTRUCT(BlueprintType)
struct FShotgunDefault
{
    GENERATED_USTRUCT_BODY()

public:
    ///Bullets
    UPROPERTY(EditAnywhere)bool bIsPiercing;
    UPROPERTY(EditAnywhere)bool bIsHoming;
    UPROPERTY(EditAnywhere)bool bIsBouncing;
    UPROPERTY(EditAnywhere)bool bExplodingBullets;
    UPROPERTY(EditAnywhere)bool bBiggerBullets;

    ///Guns
    UPROPERTY(EditAnywhere)bool bHasShield;
    UPROPERTY(EditAnywhere)bool bCanThrowGun;
    UPROPERTY(EditAnywhere)bool bConstantDashSpeed;
    UPROPERTY(EditAnywhere)float Ammo;
    UPROPERTY(EditAnywhere)float MaxAmmo;
    UPROPERTY(EditAnywhere)float Damage;
    UPROPERTY(EditAnywhere)float ProjectileSpeed;
    UPROPERTY(EditAnywhere)float ReloadSpeed;
    UPROPERTY(EditAnywhere)float FireRate;

    UPROPERTY(VisibleInstanceOnly)int32 BulletNum = 5;
    UPROPERTY(VisibleInstanceOnly)float BulletSpread = 200;
};
USTRUCT(BlueprintType)
struct FBowDefault
{
    GENERATED_USTRUCT_BODY()

public:
    ///Bullets
    UPROPERTY(EditAnywhere)bool bIsPiercing;
    UPROPERTY(EditAnywhere)bool bIsHoming;
    UPROPERTY(EditAnywhere)bool bIsBouncing;
    UPROPERTY(EditAnywhere)bool bExplodingBullets;
    UPROPERTY(EditAnywhere)bool bBiggerBullets;

    ///Guns
    UPROPERTY(EditAnywhere)bool bHasShield;
    UPROPERTY(EditAnywhere)bool bCanThrowGun;
    UPROPERTY(EditAnywhere)bool bConstantDashSpeed;
    UPROPERTY(EditAnywhere)float Ammo;
    UPROPERTY(EditAnywhere)float MaxAmmo;
    UPROPERTY(EditAnywhere)float Damage;
    UPROPERTY(EditAnywhere)float ProjectileSpeed;
    UPROPERTY(EditAnywhere)float ReloadSpeed;
    UPROPERTY(EditAnywhere)float FireRate;

    UPROPERTY(EditAnywhere)float BowDrawSpeed = 0.1;
    UPROPERTY(EditAnywhere)float MinimalDrawValue = 0.3;
};

USTRUCT(BlueprintType)
struct FUpgrades
{
    GENERATED_USTRUCT_BODY()

public:
    ///Bullets
    UPROPERTY(EditAnywhere)bool bIsPiercing;
    UPROPERTY(EditAnywhere)bool bIsHoming;
    UPROPERTY(EditAnywhere)bool bIsBouncing;
    UPROPERTY(EditAnywhere)bool bExplodingBullets;
    UPROPERTY(EditAnywhere)bool bBiggerBullets;

    ///Guns
    UPROPERTY(EditAnywhere)bool bBossOneHat;
    UPROPERTY(EditAnywhere)bool bBossTwoHat;
    UPROPERTY(EditAnywhere)bool bBoosThreeHat;

    ///Player
    UPROPERTY(EditAnywhere)bool bHasExtraLife;
    UPROPERTY(EditAnywhere)bool bReverseCubeSlowing;
    UPROPERTY(EditAnywhere)bool bHasShield;
    UPROPERTY(EditAnywhere)bool bCanThrowGun;
    UPROPERTY(EditAnywhere)bool bConstantDashSpeed;

    UPROPERTY(EditAnywhere)float Ammo;
    UPROPERTY(EditAnywhere)float MaxAmmo;
    UPROPERTY(EditAnywhere)float Damage;
    UPROPERTY(EditAnywhere)float ProjectileSpeed;
    UPROPERTY(EditAnywhere)float ReloadSpeed;
    UPROPERTY(EditAnywhere)float FireRate;

    UPROPERTY(EditAnywhere)float ProjectileSpeedMultiplier = 1;
    UPROPERTY(EditAnywhere)float DamageMultiplier = 1;
    UPROPERTY(EditAnywhere)float ReloadSpeedMultiplier = 1;
    UPROPERTY(EditAnywhere)float FireRateMultiplier = 1;

    ///Shotgun Specific
    UPROPERTY(VisibleInstanceOnly)int32 BulletNum = 5;
    UPROPERTY(VisibleInstanceOnly)float BulletSpread = 200;

    /*Will fire at 1 and add this value every tick*/
    UPROPERTY(EditAnywhere)float BowDrawSpeed = 0.01;
    /*Minimal needed draw value to fire*/
    UPROPERTY(EditAnywhere)float MinimalDrawValue = 0.3;

    FUpgrades()
    {

    }
    FUpgrades(FPistolDefault Defaults)
    {
        bIsPiercing = Defaults.bIsPiercing;
        bIsHoming = Defaults.bIsHoming;
        bIsBouncing = Defaults.bIsBouncing;
        bExplodingBullets = Defaults.bExplodingBullets;

        bHasShield = Defaults.bHasShield;
        bBiggerBullets = Defaults.bBiggerBullets;
        bCanThrowGun = Defaults.bCanThrowGun;
        bConstantDashSpeed = Defaults.bConstantDashSpeed;

        Ammo = Defaults.Ammo;
        Damage = Defaults.Damage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        ReloadSpeed = Defaults.ReloadSpeed;
    }
    FUpgrades(FShotgunDefault Defaults)
    {
        bIsPiercing = Defaults.bIsPiercing;
        bIsHoming = Defaults.bIsHoming;
        bIsBouncing = Defaults.bIsBouncing;
        bExplodingBullets = Defaults.bExplodingBullets;

        bHasShield = Defaults.bHasShield;
        bBiggerBullets = Defaults.bBiggerBullets;
        bCanThrowGun = Defaults.bCanThrowGun;
        bConstantDashSpeed = Defaults.bConstantDashSpeed;

        Ammo = Defaults.Ammo;
        Damage = Defaults.Damage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        ReloadSpeed = Defaults.ReloadSpeed;

        BulletNum = Defaults.BulletNum;
        BulletSpread = Defaults.BulletSpread;
    }
    FUpgrades(FBowDefault Defaults)
    {
        bIsPiercing = Defaults.bIsPiercing;
        bIsHoming = Defaults.bIsHoming;
        bIsBouncing = Defaults.bIsBouncing;
        bExplodingBullets = Defaults.bExplodingBullets;

        bHasShield = Defaults.bHasShield;
        bBiggerBullets = Defaults.bBiggerBullets;
        bCanThrowGun = Defaults.bCanThrowGun;
        bConstantDashSpeed = Defaults.bConstantDashSpeed;

        Ammo = Defaults.Ammo;
        Damage = Defaults.Damage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        ReloadSpeed = Defaults.ReloadSpeed;

        BowDrawSpeed = Defaults.BowDrawSpeed;
        MinimalDrawValue = Defaults.MinimalDrawValue;
    }
};

UCLASS()
class SPLITSECOND_API USplitSecondWeapons : public UObject
{
	GENERATED_BODY()
	
};
