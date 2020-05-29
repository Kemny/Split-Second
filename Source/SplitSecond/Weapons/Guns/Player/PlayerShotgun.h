// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "../../Super_Gun.h"
#include "PlayerShotgun.generated.h"

/**
 * A Shotgun
 */
UCLASS()
class SPLITSECOND_API APlayerShotgun : public ASuper_Gun
{
	GENERATED_BODY()

public:
    void FireGun() override;

protected:


};
