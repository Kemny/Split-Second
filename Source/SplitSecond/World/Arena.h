// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "../Libraries/SplitSecondArenas.h"

#include "Arena.generated.h"

DECLARE_DYNAMIC_DELEGATE(FArenaDelegate);

UENUM()
enum EObjectives
{
	Survive,
	ReachObjective,
	CaptureTheFlag,
	KillAllEnemies,
};

UCLASS()
class SPLITSECOND_API AArena : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectives>> PossibleObjectives;

public:
	AArena();
	virtual void BeginPlay() override;
	FArenaDelegate OnArenaFinished;
	void SpawnActors(const FArenaSettings& NewSettings);
	
private:
	FArenaSettings CurrentSettings;
	FTimerHandle SpawnEnemiesHandle;
	FTimerHandle SurviveHandle;

	bool bHasFlag = false;

	void SetupSurvive();
	void SetupFlag(TArray<class UActorSpawnLocationComponent*> SpawnLocations);
	void SetupObjective(TArray<class UActorSpawnLocationComponent*> SpawnLocations);
	void SetupKillAll();

	void SpawnEnemies(int32 NumberToSpawn, TArray<UActorComponent*> SpawnLocations);

	UFUNCTION() void SpawnNextEnemyWave_Survival();
	UFUNCTION() void SpawnNextEnemyWave_CaptureTheFlag();
	UFUNCTION() void SpawnNextEnemyWave_ReachTheObjective();
	UFUNCTION() void FinishSurvive();

	UFUNCTION() void AquireFlag() { bHasFlag = true; }
	UFUNCTION() void TryDeliverFlag();
	/* Called when the arenas objective is completed, to give the player the option to continue to the next one*/
	UFUNCTION() void FinishObjective();
	/* Called when player gives the input to load the next level*/
	UFUNCTION() void FinishArena();

	TSubclassOf<class UPopupMessage> PopupMessageClass;

	bool IsEnemyAtLocation(FVector Location);
};
