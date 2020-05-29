// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SuperTrap.generated.h"

UCLASS()
class SPLITSECOND_API ASuperTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	/* How much damage is applied to the player */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trap Settings")
	float Damage = 5;

	/* How often this trap goes off */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Trap Settings")
	float TrapInterval = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* TraceComp;

	ASuperTrap();
};
