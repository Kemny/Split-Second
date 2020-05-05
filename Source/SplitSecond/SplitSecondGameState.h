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
	UPROPERTY(EditAnywhere)float GlobalTimeMultiplier = 1;
	int32 CurrentLevel;

public:
	int32 GetCurrentLevel() const { return CurrentLevel; }
	void IncrementCurrentLevel() { ++CurrentLevel; }

private:

};
