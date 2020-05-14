// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawnLocation.h"
#include "Components/SceneComponent.h"
#include "RandomEnemySpawner.generated.h"

UENUM()
enum ERandomSpawnEnemy
{
  E_Charger,
  E_Shooter,
};

/* This component will spawn a random set of enemies (only chargers & shooters) at random points  */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), HideCategories = (Rendering, Tags, Replication, Activation, Cooking, Physics, LOD, Collision, Replication, AssetUserData, ComponentReplication, Variable))
class SPLITSECOND_API URandomEnemySpawner : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URandomEnemySpawner();

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
  class UBoxComponent* BoxComponent;

  UPROPERTY(EditAnywhere)
  int32 AmountToSpawn;

  FVector GetRandomPoint() const;
  TSubclassOf<AActor> GetRandomType() const;

  void SpawnEnemies(AActor* Parent);

private:

  TSubclassOf<AActor> Enemy_Charger_Class;
  TSubclassOf<AActor> Enemy_Shooter_Class;
};
