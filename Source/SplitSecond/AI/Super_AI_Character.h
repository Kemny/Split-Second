// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Super_AI_Character.generated.h"

class ASuper_Gun;

UCLASS()
class SPLITSECOND_API ASuper_AI_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASuper_AI_Character();

public:	

  /* Gun to give AI */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun Settings")
  TSubclassOf<ASuper_Gun> AIGunClass;

  /* Scale of the AI's gun */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun Settings")
  FVector GunScale;

  /* AI's current gun */
  UPROPERTY(BlueprintReadWrite, Category = "Gun Vars")
  ASuper_Gun* CurrentGun;

  /* Fires the currently equipped gun */
  void FireGun();

protected:

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

private:

  void SpawnGun();

};
