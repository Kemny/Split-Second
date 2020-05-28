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
	UPROPERTY(EditAnywhere, Category = "Music")
	USoundBase* ArenaMusic;
	UPROPERTY(EditAnywhere, Category = "Music")
	USoundBase* BossMusic;
	
	UPROPERTY(EditAnywhere)float MinTimeDilation = 0.01;
	UPROPERTY(EditAnywhere)float MaxTimeDilation = 1;


	UPROPERTY(EditDefaultsOnly, Category = "Defaults")FPistolDefault DefaultStatsForPistol;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")FShotgunDefault DefaultStatsForShotgun;
	UPROPERTY(EditDefaultsOnly, Category = "Defaults")FBowDefault DefaultStatsForBow;
	
	///TODO Remove debug Weapon class

	/*Final game will let the player pick from a menu of weapons*/
    EWeapons PlayerGun;

    UPROPERTY(EditDefaultsOnly, Category = "Arenas")
    TArray<TSubclassOf<class AArena>> PossibleArenas;
	UPROPERTY(EditDefaultsOnly, Category = "Arenas")
	TSubclassOf<class AArena> BossArena;

public:
	ASplitSecondGameMode();
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category="Weapons")
    void SetPlayerDefaultWeapon(EWeapons NewWeapon, class APlayerCharacter* PlayerPawn);

	UFUNCTION(BlueprintCallable) 
	void SpawnNextArena();
	UFUNCTION() void SpawnUpgradeScreen();
	UFUNCTION() void SpawnBossUpgradeScreen();

	UFUNCTION() void HandleArenaFinished();
	UFUNCTION() void HandleBossFinished();

	void PostLogin(APlayerController* NewPlayer) override;
	UFUNCTION() void OnPlayerDeath();
	UFUNCTION() void OnConfirmedPlayerDeath();

	UPROPERTY(EditDefaultsOnly, Category = "Game Scaling") float EnemyDamageScaler = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Game Scaling") float EnemyHealthScaler = 2;
	UPROPERTY(EditDefaultsOnly, Category = "Game Scaling") float BossDamageScaler = 1.5;
	UPROPERTY(EditDefaultsOnly, Category = "Game Scaling") float BossHealthScaler = 4;

private:
	int32 ArenaNum = 0;
	int32 Kills = 0;
	int32 ArenaHighscore = 0;

	class ASplitSecondPlayerController* SplitSecondPlayerController;
	class ASplitSecondPlayerState* SplitSecondPlayerState;
	class APlayerCharacter* SplitSecondPlayerCharacter;

	UPROPERTY() class AArena* CurrentArena;
	UPROPERTY() TArray<AActor*> SlowedActors;

	//Classes
	TSubclassOf<class ASuper_Gun> PistolClass;
	TSubclassOf<class ASuper_Gun> ShotgunClass;
	TSubclassOf<class ASuper_Gun> BowClass;
	TSubclassOf<class UPlayerUI> PlayerUIClass;
	TSubclassOf<class UTransitionScreen> TransitionScreenClass;
	TSubclassOf<class UUpgradeSelection> UpgradeSelectionClass;

	class UUserWidget* CurrentTransitionWidget;
	UAudioComponent* ArenaMusicReference = nullptr;
	UAudioComponent* BossMusicReference = nullptr;

	UFUNCTION() void PlayerSlowGame();

	float CurrentSlowValue;
	bool bGameIsSlowed = false;
	float SlowEndTime;
	void CheckSlowGame();

public:

	int32 GetArenaNum() const { return ArenaNum; }
	int32 GetHighscore() const { return ArenaHighscore; }
	int32 GetKills() const { return Kills; }
	UFUNCTION(BlueprintCallable)
	float GetSlowEndTime() const { return SlowEndTime ; }
	bool GetIsGameSlowed() const { return bGameIsSlowed; }
	float GetCurrentSlowValue() const { return CurrentSlowValue; }

	void HandleEnemyDeath();
	void AddActorToSlowedArray(AActor* ActorToSlow);

};



