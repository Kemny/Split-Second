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
    void BeginPlay() override;
    void FireGun() override;

protected:
    UPROPERTY(EditAnywhere) int32 DefaultBulletNum = 5;
    UPROPERTY(EditAnywhere) float DefaultBulletSpread = 200;

    UPROPERTY(VisibleInstanceOnly) int32 CurrentBulletNum;
    UPROPERTY(VisibleInstanceOnly) float CurrentBulletSpread = 200;


};
