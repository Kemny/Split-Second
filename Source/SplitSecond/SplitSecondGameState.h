// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

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
	int32 CurrentLevel = 1;

public:
	int32 GetCurrentLevel() const { return CurrentLevel; }
	void IncrementCurrentLevel() { ++CurrentLevel; }

private:

};
