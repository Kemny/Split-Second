// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SplitSecondPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API ASplitSecondPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

protected:
	UPROPERTY(EditAnywhere, Category = "Time Manipulation") float ActorSlowDuration = 3;
	UPROPERTY(EditAnywhere, Category = "Time Manipulation") float ActorSlowValue = 0.1;
private:
	void BeginPlay() override;
	void SetupInputComponent() override;
	void Tick(float DeltaTime) override;

	void TraceForActorsToSlow();

	UFUNCTION() void ShowDebugMenu();
	UFUNCTION() void IncreaseSlow(float Value);

	UFUNCTION() void SlowTarget();

	class ASplitSecondHUD* Hud;

	class ASplitSecondProjectile* HoveredProjectile;
	class ASuper_AI_Character* HoveredEnemy;
};
