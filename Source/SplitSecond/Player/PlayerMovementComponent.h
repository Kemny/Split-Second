// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
public:
    void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
