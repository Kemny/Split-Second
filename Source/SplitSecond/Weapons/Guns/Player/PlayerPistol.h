// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "../../Super_Gun.h"
#include "PlayerPistol.generated.h"

/**
 * A Pistol
 */
UCLASS()
class SPLITSECOND_API APlayerPistol : public ASuper_Gun
{
	GENERATED_BODY()
	
public:
	void FireGun() override;
};
