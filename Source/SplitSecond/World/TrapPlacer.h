// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TrapPlacer.generated.h"

UENUM()
enum ETrapType
{
	Trap_Spike,
	Trap_Fire,
	Trap_Cannon,
};

/* This place the selected trap on this components location */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), HideCategories = (Rendering, Tags, Replication, Activation, Cooking, Physics, LOD, Collision, Replication, AssetUserData, ComponentReplication, Variable))
class SPLITSECOND_API UTrapPlacer : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTrapPlacer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETrapType> SpawnType;

	TSubclassOf<AActor> GetCurrentTypeClass() const;
	FVector GetBoxCenter() const;

private:

	TSubclassOf<AActor> Trap_Spike_Class;
	TSubclassOf<AActor> Trap_Fire_Class;
	TSubclassOf<AActor> Trap_Cannon_Class;
};
