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

private:
	void BeginPlay() override;
	void SetupInputComponent() override;

	UFUNCTION() void ShowDebugMenu();
	UFUNCTION() void IncreaseSlow(float Value);

	class ASplitSecondHUD* Hud;
};
