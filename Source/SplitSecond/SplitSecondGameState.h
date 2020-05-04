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
	UPROPERTY(EditAnywhere)float MinTimeMultipler =0.01;
	UPROPERTY(EditAnywhere)float MaxTimeMultiplier = 1;
	int32 CurrentLevel;

public:
	UFUNCTION(BlueprintCallable)float GetGlobalTimeMultiplier() const { return GlobalTimeMultiplier; }
	UFUNCTION(BlueprintCallable)void SetGlobalTimeMultiplier(float NewValue) { GlobalTimeMultiplier = ClampTimeMultipler(NewValue); }
	UFUNCTION(BlueprintCallable)void AddToGlobalTimeMultiplier(float ValueToAdd) { GlobalTimeMultiplier = ClampTimeMultipler(ValueToAdd + GlobalTimeMultiplier); }

	int32 GetCurrentLevel() const { return CurrentLevel; }
	void IncrementCurrentLevel() { ++CurrentLevel; }

private:
	//So that you don't speed up time or go into negatives
	FORCEINLINE float ClampTimeMultipler(const float& Value)
	{
		return FMath::Clamp<float>(Value, MinTimeMultipler, MaxTimeMultiplier);
	}
};
