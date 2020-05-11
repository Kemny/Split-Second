// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "Libraries/SplitSecondWeapons.h" ///Structs cannot be forward declared

#include "SplitSecondPlayerState.generated.h"

UCLASS()
class SPLITSECOND_API ASplitSecondPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
    UPROPERTY(VisibleInstanceOnly)
    FUpgrades CurrentUpgrades; 
    
    void SetDefaultWeapon(EWeapons NewWeapon);

};
