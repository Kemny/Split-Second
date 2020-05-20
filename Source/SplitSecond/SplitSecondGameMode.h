// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "Libraries/SplitSecondWeapons.h" ///Structs cannot be forward declared

#include "SplitSecondGameMode.generated.h"

UCLASS(minimalapi)
class ASplitSecondGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)float MinTimeDilation = 0.01;
	UPROPERTY(EditAnywhere)float MaxTimeDilation = 1;


	UPROPERTY(EditDefaultsOnly, Category = "Defaults")FPistolDefault DefaultStatsForPistol;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")FShotgunDefault DefaultStatsForShotgun;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")FBowDefault DefaultStatsForBow;
	
	///TODO Remove debug Weapon class

	/*Final game will let the player pick from a menu of weapons*/
    UPROPERTY(EditDefaultsOnly, Category = "Defaults")
    TEnumAsByte<EWeapons> Temp_StartingGun;

    UPROPERTY(EditDefaultsOnly, Category = "Arenas")
    TArray<TSubclassOf<class AArena>> PossibleArenas;
	UPROPERTY(EditDefaultsOnly, Category = "Arenas")
	TSubclassOf<class AArena> Boss1Arena;
	UPROPERTY(EditDefaultsOnly, Category = "Arenas")
	TSubclassOf<class AArena> Boss2Arena;
	UPROPERTY(EditDefaultsOnly, Category = "Arenas")
	TSubclassOf<class AArena> Boss3Arena;

	// Boss 1 Time skip multiplier multiplies all projectiles current velocity
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss 1 Settings")
	float Boss1SkipAmmount = 2;
	

public:
	ASplitSecondGameMode();
	void BeginPlay() override;

    void SetDefaultWeapon(EWeapons NewWeapon, TSubclassOf<class ASuper_Gun> Pistol);

	UFUNCTION() void SpawnNextArena();

	void PostLogin(APlayerController* NewPlayer) override;
	UFUNCTION() void OnPlayerDeath();
	UFUNCTION() void OnConfirmedPlayerDeath();

	UFUNCTION(BlueprintCallable, Category = "Time Functions") void StartTimeSkip();

private:
	TSubclassOf<class ASuper_Gun> PistolClass;
	TSubclassOf<class ASuper_Gun> ShotgunClass;
	TSubclassOf<class ASuper_Gun> BowClass;
};



