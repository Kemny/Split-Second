// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BossAddsSpawnLocation.generated.h"

UENUM()
enum EBossAddsSpawnType
{
	Boss_Enemy_Charger,
	Boss_Enemy_Healer,
	Boss_Enemy_Shooter,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), HideCategories = (Rendering, Tags, Replication, Activation, Cooking, Physics, LOD, Collision, Replication, AssetUserData, ComponentReplication, Variable))
class SPLITSECOND_API UBossAddsSpawnLocation : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossAddsSpawnLocation();

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EBossAddsSpawnType> SpawnType;

	TSubclassOf<AActor> GetCurrentTypeClass() const;
	TSubclassOf<AActor> GetActorClass;
	FVector GetBoxCenter() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComponent;

	bool HasEnemyInBounds();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	TSubclassOf<AActor> Enemy_Charger_Class;
	TSubclassOf<AActor> Enemy_Healer_Class;
	TSubclassOf<AActor> Enemy_Shooter_Class;

	bool bEnemyInBounds;
};
