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
    UPROPERTY(EditAnywhere)bool bIsPiercing = false;
    UPROPERTY(EditAnywhere)bool bIsHoming = false;
    UPROPERTY(EditAnywhere)bool bIsBouncing = false;
    UPROPERTY(EditAnywhere)bool bExplodingBullets = false;
    UPROPERTY(EditAnywhere)bool bExtraLife = false;

    ///Guns
    UPROPERTY(EditAnywhere)int32 MaxAmmo = 10;
    UPROPERTY(EditAnywhere)float Damage = 10;
    UPROPERTY(EditAnywhere)float ProjectileSpeed = 1500;
    UPROPERTY(EditAnywhere)float ReloadSpeed = 0.6;
    UPROPERTY(EditAnywhere)float FireRate = 0.3;

    ///Player
    UPROPERTY(EditAnywhere)bool bCanThrowGun = false;
    UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 MaxJumps = 1;
};
USTRUCT(BlueprintType)
struct FShotgunDefault
{
    GENERATED_USTRUCT_BODY()

public:
    ///Bullets
    UPROPERTY(EditAnywhere)bool bIsPiercing = false;
    UPROPERTY(EditAnywhere)bool bIsHoming = false;
    UPROPERTY(EditAnywhere)bool bIsBouncing = false;
    UPROPERTY(EditAnywhere)bool bExplodingBullets = false;
    UPROPERTY(EditAnywhere)bool bExtraLife = false;

    ///Guns
    UPROPERTY(EditAnywhere)int32 MaxAmmo = 10;
    UPROPERTY(EditAnywhere)float Damage = 10;
    UPROPERTY(EditAnywhere)float ProjectileSpeed = 1500;
    UPROPERTY(EditAnywhere)float ReloadSpeed = 0.6;
    UPROPERTY(EditAnywhere)float FireRate = 0.3;

    ///Player
    UPROPERTY(EditAnywhere)bool bCanThrowGun = false;

    UPROPERTY(EditAnywhere)int32 BulletNum = 5;
    UPROPERTY(EditAnywhere)float BulletSpread = 200;
};
USTRUCT(BlueprintType)
struct FBowDefault
{
    GENERATED_USTRUCT_BODY()

public:
    ///Bullets
    UPROPERTY(EditAnywhere)bool bIsPiercing = false;
    UPROPERTY(EditAnywhere)bool bIsHoming = false;
    UPROPERTY(EditAnywhere)bool bIsBouncing = false;
    UPROPERTY(EditAnywhere)bool bExplodingBullets = false;
    UPROPERTY(EditAnywhere)bool bExtraLife = false;

    ///Guns
    UPROPERTY(EditAnywhere)int32 MaxAmmo = 10;
    UPROPERTY(EditAnywhere)float Damage = 10;
    UPROPERTY(EditAnywhere)float ProjectileSpeed = 1500;
    UPROPERTY(EditAnywhere)float ReloadSpeed = 0.6;
    UPROPERTY(EditAnywhere)float FireRate = 0.3;

    ///Player
    UPROPERTY(EditAnywhere)bool bCanThrowGun = false;

    UPROPERTY(EditAnywhere)float BowDrawSpeed = 0.01;
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

    ///Guns
    UPROPERTY(EditAnywhere)bool bBossOneHat = false;
    UPROPERTY(EditAnywhere)bool bBossTwoHat = false;
    UPROPERTY(EditAnywhere)bool bBossThreeHat = false;

    ///Player
    UPROPERTY(EditAnywhere)bool bHasExtraLife = false;
    UPROPERTY(EditAnywhere)bool bCanThrowGun = false;

    UPROPERTY(EditAnywhere)int32 Ammo = 999;
    UPROPERTY(EditAnywhere)int32 MaxAmmo = 999;
    UPROPERTY(EditAnywhere)float Damage = 999;
    UPROPERTY(EditAnywhere)float ProjectileSpeed = 999;
    UPROPERTY(EditAnywhere)float ReloadSpeed = 999;
    UPROPERTY(EditAnywhere)float FireRate = 999;

    ///Pistol Specific
    UPROPERTY(EditAnywhere)int32 MaxJumps = 1;

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

        bCanThrowGun = Defaults.bCanThrowGun;

        MaxAmmo = Defaults.MaxAmmo;
        Ammo = MaxAmmo;
        Damage = Defaults.Damage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        ReloadSpeed = Defaults.ReloadSpeed;
        FireRate = Defaults.FireRate;

        MaxJumps = Defaults.MaxJumps;
    }
    FUpgrades(FShotgunDefault Defaults)
    {
        bIsPiercing = Defaults.bIsPiercing;
        bIsHoming = Defaults.bIsHoming;
        bIsBouncing = Defaults.bIsBouncing;
        bExplodingBullets = Defaults.bExplodingBullets;

        bCanThrowGun = Defaults.bCanThrowGun;

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
        bCanThrowGun = Defaults.bCanThrowGun;

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
