// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

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
