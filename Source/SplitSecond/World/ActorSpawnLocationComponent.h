// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ActorSpawnLocationComponent.generated.h"

UENUM()
enum ESpawnType
{
	Objective_Flag,
	Objective_FlagTarget,
	Objective_PlayerTarget,
	Player_Location
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories = (Rendering, Tags, Replication, Activation, Cooking, Physics, LOD, Collision, Replication, AssetUserData, ComponentReplication, Variable ) )
class SPLITSECOND_API UActorSpawnLocationComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ESpawnType> SpawnType;

	TSubclassOf<AActor> GetCurrentTypeClass() const;
	TSubclassOf<AActor> GetActorClass;

private:
	UActorSpawnLocationComponent();

	TSubclassOf<AActor> Objective_Flag_Class;
	TSubclassOf<AActor> Objective_FlagTarget_Class;
	TSubclassOf<AActor> Objective_PlayerTarget_Class;
};
