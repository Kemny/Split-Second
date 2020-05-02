// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SplitSecondGameState.generated.h"

/*
 * GameState, contains most global variables
*/

UCLASS()
class SPLITSECOND_API ASplitSecondGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleInstanceOnly)float GlobalTimeMultiplier;
	UPROPERTY(VisibleInstanceOnly)int32 CurrentLevel;

public:
	float GetGlobalTimeMultiplier() const { return GlobalTimeMultiplier; }
	void SetGlobalTimeMultiplier(float NewValue) { GlobalTimeMultiplier = NewValue; }

	int32 GetCurrentLevel() const { return CurrentLevel; }
	void IncrementCurrentLevel() { ++CurrentLevel; }
};
