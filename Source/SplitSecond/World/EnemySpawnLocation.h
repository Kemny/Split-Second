// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "EnemySpawnLocation.generated.h"


UENUM()
enum EEnemySpawnType
{
  Enemy_Charger,
  Enemy_Healer,
  Enemy_Shooter,
  Enemy_Turret,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), HideCategories = (Rendering, Tags, Replication, Activation, Cooking, Physics, LOD, Collision, Replication, AssetUserData, ComponentReplication, Variable))
class SPLITSECOND_API UEnemySpawnLocation : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemySpawnLocation();

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TEnumAsByte<EEnemySpawnType> SpawnType;

  TSubclassOf<AActor> GetCurrentTypeClass() const;
  TSubclassOf<AActor> GetActorClass;
  FVector GetBoxCenter() const;

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
  class UBoxComponent* BoxComponent;

private:

  TSubclassOf<AActor> Enemy_Charger_Class;
  TSubclassOf<AActor> Enemy_Healer_Class;
  TSubclassOf<AActor> Enemy_Shooter_Class;
  TSubclassOf<AActor> Enemy_Turret_Class;

};
