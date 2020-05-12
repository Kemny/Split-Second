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
    UPROPERTY(EditAnywhere)int32 MaxAmmo;
    UPROPERTY(EditAnywhere)float Damage;
    UPROPERTY(EditAnywhere)float ProjectileSpeed;
    UPROPERTY(EditAnywhere)float ReloadSpeed;
    UPROPERTY(EditAnywhere)float FireRate;
    UPROPERTY(EditAnywhere)bool bReloadConstant;
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
    UPROPERTY(EditAnywhere)int32 MaxAmmo;
    UPROPERTY(EditAnywhere)float Damage;
    UPROPERTY(EditAnywhere)float ProjectileSpeed;
    UPROPERTY(EditAnywhere)float ReloadSpeed;
    UPROPERTY(EditAnywhere)float FireRate;
    UPROPERTY(EditAnywhere)bool bReloadConstant;

    UPROPERTY(EditAnywhere)int32 BulletNum = 5;
    UPROPERTY(EditAnywhere)float BulletSpread = 200;
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
    UPROPERTY(EditAnywhere)int32 MaxAmmo;
    UPROPERTY(EditAnywhere)float Damage;
    UPROPERTY(EditAnywhere)float ProjectileSpeed;
    UPROPERTY(EditAnywhere)float ReloadSpeed;
    UPROPERTY(EditAnywhere)float FireRate;
    UPROPERTY(EditAnywhere)bool bReloadConstant;

    UPROPERTY(EditAnywhere)float BowDrawSpeed = 0.1;
    UPROPERTY(EditAnywhere)float MinimalDrawValue = 0.3;
};

USTRUCT(BlueprintType)
struct FUpgrades
{
    GENERATED_USTRUCT_BODY()

public:
    ///Bullets
    UPROPERTY(EditAnywhere)bool bIsPiercing = false;
    UPROPERTY(EditAnywhere)bool bIsHoming = false;
    UPROPERTY(EditAnywhere)bool bIsBouncing = false;
    UPROPERTY(EditAnywhere)bool bExplodingBullets = false;
    UPROPERTY(EditAnywhere)bool bBiggerBullets = false;

    ///Guns
    UPROPERTY(EditAnywhere)bool bBossOneHat = false;
    UPROPERTY(EditAnywhere)bool bBossTwoHat = false;
    UPROPERTY(EditAnywhere)bool bBoosThreeHat = false;
    UPROPERTY(EditAnywhere)bool bReloadConstant = true;

    ///Player
    UPROPERTY(EditAnywhere)bool bHasExtraLife = false;
    UPROPERTY(EditAnywhere)bool bReverseCubeSlowing = false;
    UPROPERTY(EditAnywhere)bool bHasShield = false;
    UPROPERTY(EditAnywhere)bool bCanThrowGun = false;
    UPROPERTY(EditAnywhere)bool bConstantDashSpeed = false;

    UPROPERTY(EditAnywhere)int32 Ammo = 999;
    UPROPERTY(EditAnywhere)int32 MaxAmmo = 999;
    UPROPERTY(EditAnywhere)float Damage = 999;
    UPROPERTY(EditAnywhere)float ProjectileSpeed = 999;
    UPROPERTY(EditAnywhere)float ReloadSpeed = 999;
    UPROPERTY(EditAnywhere)float FireRate = 999;

    UPROPERTY(EditAnywhere)float ProjectileSpeedMultiplier = 1;
    UPROPERTY(EditAnywhere)float DamageMultiplier = 1;
    UPROPERTY(EditAnywhere)float ReloadSpeedMultiplier = 1;
    UPROPERTY(EditAnywhere)float FireRateMultiplier = 1;

    ///Shotgun Specific
    UPROPERTY(EditAnywhere)int32 BulletNum = 5;
    UPROPERTY(EditAnywhere)float BulletSpread = 200;

    ///Bow Specific

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
        bReloadConstant = Defaults.bReloadConstant;

        MaxAmmo = Defaults.MaxAmmo;
        Ammo = MaxAmmo;
        Damage = Defaults.Damage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        ReloadSpeed = Defaults.ReloadSpeed;
        FireRate = Defaults.FireRate;
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
        bReloadConstant = Defaults.bReloadConstant;

        MaxAmmo = Defaults.MaxAmmo;
        Ammo = MaxAmmo;
        Damage = Defaults.Damage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        ReloadSpeed = Defaults.ReloadSpeed;
        FireRate = Defaults.FireRate;

        BulletNum = Defaults.BulletNum;
        BulletSpread = Defaults.BulletSpread;
    }
    FUpgrades(FBowDefault Defaults)
    {
        bIsPiercing = Defaults.bIsPiercing;
        bIsHoming = Defaults.bIsHoming;
        bIsBouncing = Defaults.bIsBouncing;
        bExplodingBullets = Defaults.bExplodingBullets;
        bReloadConstant = Defaults.bReloadConstant;

        bHasShield = Defaults.bHasShield;
        bBiggerBullets = Defaults.bBiggerBullets;
        bCanThrowGun = Defaults.bCanThrowGun;
        bConstantDashSpeed = Defaults.bConstantDashSpeed;

        MaxAmmo = Defaults.MaxAmmo;
        Ammo = MaxAmmo;
        Damage = Defaults.Damage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        ReloadSpeed = Defaults.ReloadSpeed;
        FireRate = Defaults.FireRate;

        BowDrawSpeed = Defaults.BowDrawSpeed;
        MinimalDrawValue = Defaults.MinimalDrawValue;
    }

};

UCLASS()
class SPLITSECOND_API USplitSecondWeapons : public UObject
{
	GENERATED_BODY()
	
};
