// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arena.generated.h"

DECLARE_DYNAMIC_DELEGATE(FArenaDelegate);

UENUM()
enum EObjectives
{
	Survive,
	ReachObjective,
	CaptureTheFlag,
	KillAllEnemies,
	KillBoss,
	Tutorial
};

/*
	Some parameters are objetive Specific
*/
USTRUCT(BlueprintType)
struct FArenaSettings
{
	GENERATED_USTRUCT_BODY()

public:
	/* SURVIVAL ONLY */
	UPROPERTY(EditAnywhere)int32 SurviveTime = 180;
	/* NOT APPLIED TO KILL ALL ENEMIES */
	UPROPERTY(EditAnywhere)int32 WaveInterval = 60;
	UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 EnemiesPerWaveMin = 1;
	UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 EnemiesPerWaveMax = 1;
	/*KILL ALL ENEMIES ONLY*/
	UPROPERTY(EditAnywhere)int32 EnemyTotal = 10;

};

UCLASS()
class SPLITSECOND_API AArena : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USceneComponent* Root;
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USceneComponent* PlayerStartLocation;

	//Should Be 100 - 200 Unit Above Ground
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USceneComponent* FlagScene;
	//Should Be 100 - 200 Unit Above Ground
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USceneComponent* FlagTargetScene;
	//Should Be 1 Unit Above Ground
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USceneComponent* LocationTargetScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Arena")
	TMap<TEnumAsByte<EObjectives>, FArenaSettings> Objectives;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Random")
	int32 MAX_ATTEMPTS = 500;

public:
	AArena();
	void SpawnActors();
	void Tick(float DeltaTime) override;

	FArenaDelegate OnArenaFinished;

private:
	TArray<class AActor*> SpawnedEnemies;
	TMap<class UEnemySpawnLocation* , class AActor*> SpawnedTurrets;
	class AFlagTarget* SpawnedFlagTarget;
	class APlayerCharacter* PlayerPawn;

	EObjectives CurrentObjective;
	FArenaSettings* CurrentSettings;

	bool bHasFlag = false;
	bool ObjectiveFinished = false;

	UFUNCTION() void SetupSurvive();
	UFUNCTION() void SetupFlag();
	UFUNCTION() void SetupObjective();
	UFUNCTION() void SetupKillAll();
	UFUNCTION() void SetupKillBoss();
	UFUNCTION() void SetupTutorial();

	void SpawnEnemies(int32 NumberToSpawn, TArray<UEnemySpawnLocation*> SpawnLocations);

	UFUNCTION() void AquireFlag() { bHasFlag = true; }
	UFUNCTION() void TryDeliverFlag();

	UFUNCTION() void SpawnKillAllWave();

	/* Called when the arenas objective is completed, to give the player the option to continue to the next one*/
	UFUNCTION() void FinishObjective();
	/* Called when player gives the input to load the next level*/
	UFUNCTION() void FinishArena();

	UFUNCTION() void OnTurretDeath(AActor* KilledAI);
	UFUNCTION() void HandleEnemyDeath(AActor* KilledAI);
	UFUNCTION() void OnBossDeath(AActor* KilledAI);

	TSubclassOf<class UPopupMessage> PopupMessageClass;

	// Enemy Wave
	void CheckSpawnEnemyWaveTimer();
	float WaveTimerTargetTime;
	bool bSpawningEnemyWaves = false;

	// Survival
	void CheckSurviveTimer();
	float SurviveTargetTime;
	bool bSurviving = false;
};
