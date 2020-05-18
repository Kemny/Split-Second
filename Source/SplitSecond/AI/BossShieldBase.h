// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossShieldBase.generated.h"

UCLASS()
class SPLITSECOND_API ABossShieldBase : public AActor
{
	GENERATED_BODY()

public:

	/* Called when shield is spawned */
	UFUNCTION(BlueprintImplementableEvent, Category = "Shield Events")
	void ActivateShield();

	/* Called when shield is despawning */
	UFUNCTION(BlueprintImplementableEvent, Category = "Shield Events")
	void DeactivateShield();

	void ConstructShield();

};
