// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Libraries/SplitSecondWeapons.h"

#include "SplitSecondGameInstance.generated.h"

UCLASS()
class SPLITSECOND_API USplitSecondGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	/*Weapon that will be set when not starting from main menu*/
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EWeapons> StartingWeapon;

public:
	UFUNCTION(BlueprintCallable)
	void SetStartingWeapon(EWeapons NewWeapon) { StartingWeapon = NewWeapon; }
	UFUNCTION(BlueprintCallable)
	EWeapons GetStartingWeapon() const { return StartingWeapon; }
};
