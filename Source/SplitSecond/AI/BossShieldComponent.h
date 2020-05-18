// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BossShieldComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPLITSECOND_API UBossShieldComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	void ActivateShield();

	void DeactivateShield();

	/* How long the shield is up for */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shield Settings")
	float ShieldUpTime = 3;

	/* Shield class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shield Settings")
	TSubclassOf<class ABossShieldBase> ShieldToSpawn;

private:

	class ABossShieldBase* CurrentShield;

};
