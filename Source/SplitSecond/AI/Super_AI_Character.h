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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

  /* AI's current gun */
  UPROPERTY(BlueprintReadWrite)
  ASuper_Gun* CurrentGun;

  /* Gets the AI's ID */
  UFUNCTION(BlueprintPure, Category = "AI Functions")
  int32 GetID();

private:

  int32 ID;
};
