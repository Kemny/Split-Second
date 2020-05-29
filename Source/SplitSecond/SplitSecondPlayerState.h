// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

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
    UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
    FUpgrades CurrentStats; 

};
