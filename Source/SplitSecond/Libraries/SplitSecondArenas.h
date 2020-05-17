// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SplitSecondArenas.generated.h"

USTRUCT(BlueprintType)
struct FSurvival
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)int32 SurviveTime = 180;
	UPROPERTY(EditAnywhere)int32 WaveInterval = 1;
	UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 EnemiesPerWaveMin = 1;
	UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 EnemiesPerWaveMax = 1;
};
USTRUCT(BlueprintType)
struct FCaptureTheFlag
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)int32 WaveInterval = 1;
	UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 EnemiesPerWaveMin = 1;
	UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 EnemiesPerWaveMax = 1;
};
USTRUCT(BlueprintType)
struct FReachObjective
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)int32 WaveInterval = 1;
	UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 EnemiesPerWaveMin = 1;
	UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 EnemiesPerWaveMax = 1;
};
USTRUCT(BlueprintType)
struct FKillAllEnemies
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 EnemiesPerWaveMin = 1;
	UPROPERTY(EditAnywhere, meta = (UIMin = '1'))int32 EnemiesPerWaveMax = 1;
};


USTRUCT(BlueprintType)
struct FArenaSettings
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere)FSurvival SurvivalSettings;
	UPROPERTY(EditAnywhere)FCaptureTheFlag CaptureTheFlagSettings;
	UPROPERTY(EditAnywhere)FReachObjective ReachObjectiveSettings;
	UPROPERTY(EditAnywhere)FKillAllEnemies KillAllEnemiesSettings;
};

UCLASS()
class SPLITSECOND_API USplitSecondArenas : public UObject
{
	GENERATED_BODY()
	
};
