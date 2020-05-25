// This project falls under CC-BY-SA lisence

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
	KillBoss
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

public:
	AArena();
	void SpawnActors();

	FArenaDelegate OnArenaFinished;
	FArenaDelegate OnEnemyDeath;

private:
	TArray<class ASuper_AI_Character*> SpawnedEnemies;
	TMap<class UEnemySpawnLocation* , class ASuper_AI_Character*> SpawnedTurrets;
	class AFlagTarget* SpawnedFlagTarget;
	class APlayerCharacter* PlayerPawn;

	EObjectives CurrentObjective;
	FTimerHandle SpawnEnemiesHandle;
	FTimerHandle SurviveHandle;

	bool bHasFlag = false;

	void SetupFlag();
	void SetupObjective();
	UFUNCTION() void SpawnKillAllWave();
	void SetupKillBoss();

	void SpawnEnemies(int32 NumberToSpawn, TArray<UActorComponent*> SpawnLocations);

	UFUNCTION() void SpawnNextEnemyWave();

	UFUNCTION() void AquireFlag() { bHasFlag = true; }
	UFUNCTION() void TryDeliverFlag();
	/* Called when the arenas objective is completed, to give the player the option to continue to the next one*/
	UFUNCTION() void FinishObjective();
	/* Called when player gives the input to load the next level*/
	UFUNCTION() void FinishArena();

	UFUNCTION() void OnTurretDeath(ASuper_AI_Character* KilledAI);
	UFUNCTION() void HandleEnemyDeath(ASuper_AI_Character* KilledAI);
	UFUNCTION() void OnBossDeath(ASuper_AI_Character* KilledAI);

	TSubclassOf<class UPopupMessage> PopupMessageClass;
};
