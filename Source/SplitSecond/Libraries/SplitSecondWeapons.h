// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

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
    UPROPERTY(EditAnywhere)bool Flaming = false;

    ///Guns
    UPROPERTY(EditAnywhere)float Damage = 10;
    UPROPERTY(EditAnywhere)float ProjectileSpeed = 1500;
    UPROPERTY(EditAnywhere)float FireRate = 0.3;

    ///Player
    UPROPERTY(EditAnywhere)float MaxHealth = 100;
    UPROPERTY(EditAnywhere)float Health = 100;
    UPROPERTY(EditAnywhere)bool bCanThrowGun = false;
    UPROPERTY(EditAnywhere)float ActorSlowValue = 0.1;
    UPROPERTY(EditAnywhere)float ActorSlowDuration = 3;
    UPROPERTY(EditAnywhere)float GameSlowDuration = 3;
    UPROPERTY(EditAnywhere)float GameSlowValue = 0.4;
    UPROPERTY(EditAnywhere)float GameSlowCooldown = 10;
    UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 MaxJumps = 2;
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
    UPROPERTY(EditAnywhere)bool Flaming = false;

    ///Guns
    UPROPERTY(EditAnywhere)float Damage = 10;
    UPROPERTY(EditAnywhere)float ProjectileSpeed = 1500;
    UPROPERTY(EditAnywhere)float FireRate = 0.3;

    ///Player
    UPROPERTY(EditAnywhere)float MaxHealth = 100;
    UPROPERTY(EditAnywhere)float Health = 100;
    UPROPERTY(EditAnywhere)bool bCanThrowGun = false;
    UPROPERTY(EditAnywhere)float ActorSlowValue = 0.1;
    UPROPERTY(EditAnywhere)float ActorSlowDuration = 3;
    UPROPERTY(EditAnywhere)float GameSlowDuration = 3;
    UPROPERTY(EditAnywhere)float GameSlowValue = 0.4;
    UPROPERTY(EditAnywhere)float GameSlowCooldown = 10;

    UPROPERTY(EditAnywhere)int32 BulletNum = 5;
    UPROPERTY(EditAnywhere)float BulletSpread = 2000;
};
USTRUCT(BlueprintType)
struct FBowDefault
{
    GENERATED_USTRUCT_BODY()

public:
    ///Bullets
    UPROPERTY(EditAnywhere)bool bIsPiercing = true;
    UPROPERTY(EditAnywhere)bool bIsHoming = true;
    UPROPERTY(EditAnywhere)bool bIsBouncing = false;
    UPROPERTY(EditAnywhere)bool bExplodingBullets = false;
    UPROPERTY(EditAnywhere)bool bExtraLife = false;
    UPROPERTY(EditAnywhere)bool Flaming = false;

    ///Guns
    UPROPERTY(EditAnywhere)float Damage = 10;
    UPROPERTY(EditAnywhere)float ProjectileSpeed = 1500;

    ///Player
    UPROPERTY(EditAnywhere)float MaxHealth = 100;
    UPROPERTY(EditAnywhere)float Health = 100;
    UPROPERTY(EditAnywhere)bool bCanThrowGun = false;
    UPROPERTY(EditAnywhere)float ActorSlowValue = 0.1;
    UPROPERTY(EditAnywhere)float ActorSlowDuration = 3;
    UPROPERTY(EditAnywhere)float GameSlowDuration = 3;
    UPROPERTY(EditAnywhere)float GameSlowValue = 0.4;
    UPROPERTY(EditAnywhere)float GameSlowCooldown = 10;

    UPROPERTY(EditAnywhere)float BowDrawSpeed = 0.01;
    UPROPERTY(EditAnywhere)float BowDrawDamageMultiplier = 10;
    UPROPERTY(EditAnywhere)float BowGravityMultiplier = 20;
};

USTRUCT(BlueprintType)
struct FUpgrades
{
    GENERATED_USTRUCT_BODY()

public:
    ///Bullets
    UPROPERTY(EditAnywhere, BlueprintReadWrite)bool bIsPiercing = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)bool bIsHoming = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)bool bIsBouncing = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)bool bExplodingBullets = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)bool Flaming = false;

    ///Player
    UPROPERTY(EditAnywhere, BlueprintReadWrite)float MaxHealth = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)float Health = 100;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)bool bHasExtraLife = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)bool bCanThrowGun = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)float ActorSlowValue = 0.1;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)float ActorSlowDuration = 3;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)float GameSlowDuration = 3;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)float GameSlowValue = 0.4;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)float GameSlowCooldown = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)float Damage = 999;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)float ProjectileSpeed = 999;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)float FireRate = 999;

    ///Pistol Specific
    UPROPERTY(EditAnywhere, BlueprintReadWrite)int32 MaxJumps = 2;

    ///Shotgun Specific
    UPROPERTY(EditAnywhere, BlueprintReadWrite)int32 BulletNum = 5;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)int32 BulletSpread = 200;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)int32 BulletSpreadDefault = 200;

    ///Bow Specific
    /*Will fire at 1 and add this value every tick*/
    UPROPERTY(EditAnywhere, BlueprintReadWrite)float BowDrawSpeed = 0.01;
    UPROPERTY(EditAnywhere)float BowDrawDamageMultiplier = 10;
    UPROPERTY(EditAnywhere)float BowGravityMultiplier = 20;

    FUpgrades()
    {

    }
    FUpgrades(FPistolDefault Defaults)
    {
        bIsPiercing = Defaults.bIsPiercing;
        bIsHoming = Defaults.bIsHoming;
        bIsBouncing = Defaults.bIsBouncing;
        bExplodingBullets = Defaults.bExplodingBullets;
        bHasExtraLife = Defaults.bExtraLife;
        Flaming = Defaults.Flaming;

        MaxHealth = Defaults.MaxHealth;
        Health = Defaults.MaxHealth;
        bCanThrowGun = Defaults.bCanThrowGun;
        ActorSlowValue = Defaults.ActorSlowValue;
        ActorSlowDuration = Defaults.ActorSlowDuration;
        GameSlowDuration = Defaults.GameSlowDuration;
        GameSlowValue = Defaults.GameSlowValue;
        GameSlowCooldown = Defaults.GameSlowCooldown;

        Damage = Defaults.Damage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        FireRate = Defaults.FireRate;

        MaxJumps = Defaults.MaxJumps;
    }
    FUpgrades(FShotgunDefault Defaults)
    {
        bIsPiercing = Defaults.bIsPiercing;
        bIsHoming = Defaults.bIsHoming;
        bIsBouncing = Defaults.bIsBouncing;
        bExplodingBullets = Defaults.bExplodingBullets;
        bHasExtraLife = Defaults.bExtraLife;
        Flaming = Defaults.Flaming;

        MaxHealth = Defaults.MaxHealth;
        Health = Defaults.MaxHealth;
        bCanThrowGun = Defaults.bCanThrowGun;
        ActorSlowValue = Defaults.ActorSlowValue;
        ActorSlowDuration = Defaults.ActorSlowDuration;
        GameSlowDuration = Defaults.GameSlowDuration;
        GameSlowValue = Defaults.GameSlowValue;
        GameSlowCooldown = Defaults.GameSlowCooldown;

        Damage = Defaults.Damage;
        ProjectileSpeed = Defaults.ProjectileSpeed;
        FireRate = Defaults.FireRate;

        BulletNum = Defaults.BulletNum;
        BulletSpreadDefault = Defaults.BulletSpread;
        BulletSpread = Defaults.BulletSpread;
    }
    FUpgrades(FBowDefault Defaults)
    {
        bIsPiercing = Defaults.bIsPiercing;
        bIsHoming = Defaults.bIsHoming;
        bIsBouncing = Defaults.bIsBouncing;
        bExplodingBullets = Defaults.bExplodingBullets;
        bHasExtraLife = Defaults.bExtraLife;
        Flaming = Defaults.Flaming;

        MaxHealth = Defaults.MaxHealth;
        Health = Defaults.MaxHealth;
        bCanThrowGun = Defaults.bCanThrowGun;
        ActorSlowValue = Defaults.ActorSlowValue;
        ActorSlowDuration = Defaults.ActorSlowDuration;
        GameSlowDuration = Defaults.GameSlowDuration;
        GameSlowValue = Defaults.GameSlowValue;
        GameSlowCooldown = Defaults.GameSlowCooldown;

        Damage = Defaults.Damage;
        ProjectileSpeed = Defaults.ProjectileSpeed;

        BowDrawSpeed = Defaults.BowDrawSpeed;
        BowDrawDamageMultiplier = Defaults.BowDrawDamageMultiplier;
        BowGravityMultiplier = Defaults.BowGravityMultiplier;
    }

};

UCLASS()
class SPLITSECOND_API USplitSecondWeapons : public UObject
{
    GENERATED_BODY()

};
