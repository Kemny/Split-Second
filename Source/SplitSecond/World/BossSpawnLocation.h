// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BossSpawnLocation.generated.h"

UENUM()
enum EBossSpawnType
{
	Boss_1,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), HideCategories = (Rendering, Tags, Replication, Activation, Cooking, Physics, LOD, Collision, Replication, AssetUserData, ComponentReplication, Variable))
class SPLITSECOND_API UBossSpawnLocation : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossSpawnLocation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EBossSpawnType> SpawnType;

	TSubclassOf<AActor> GetCurrentTypeClass() const;
	TSubclassOf<AActor> GetActorClass;
	FVector GetBoxCenter() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComponent;

private:
	TSubclassOf<AActor> Boss_1_Class;
};
