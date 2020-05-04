// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

/**
 * Movement Component Where Speed is affected by time manipulation
 */
UCLASS()
class SPLITSECOND_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    bool DoJump(bool bReplayingMoves) override;
};
