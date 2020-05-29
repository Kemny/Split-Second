// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

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
    UPROPERTY(EditAnywhere) int32 BulletNum = 5;
    UPROPERTY(EditAnywhere) float BulletSpread = 2000;
};
