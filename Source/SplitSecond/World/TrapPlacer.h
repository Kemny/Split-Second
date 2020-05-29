// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TrapPlacer.generated.h"

/* This place the selected trap on this components location */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), HideCategories = (Rendering, Tags, Replication, Activation, Cooking, Physics, LOD, Collision, Replication, AssetUserData, ComponentReplication, Variable))
class SPLITSECOND_API UTrapPlacer : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTrapPlacer();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UChildActorComponent* TrapToPlace;

};
