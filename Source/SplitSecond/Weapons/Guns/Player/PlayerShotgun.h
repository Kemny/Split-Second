// This project falls under CC-BY-SA lisence

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
