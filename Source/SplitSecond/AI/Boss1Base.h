// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "../AI/Super_Boss.h"
#include "Boss1Base.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API ABoss1Base : public ASuper_Boss
{
	GENERATED_BODY()

public:

	/* Delay before the boss teleports */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Settings")
	float TeleportDelay = 2;

	/* Delay before the shoots his gun */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss Settings")
	float GunShotDelay = 3;
};
