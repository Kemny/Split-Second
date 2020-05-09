// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "../../Super_Gun.h"
#include "AIShotgun.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API AAIShotgun : public ASuper_Gun
{
	GENERATED_BODY()

public:
    void FireGun() override;

protected:
    UPROPERTY(EditAnywhere) int32 DefaultBulletNum = 5;
    UPROPERTY(EditAnywhere) float DefaultBulletSpread = 200;
};
