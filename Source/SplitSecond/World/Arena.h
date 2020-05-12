// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arena.generated.h"

UENUM()
enum EObjectives
{
	Survive,
	ReachObjective,
	CaptureTheFlag,
	KillAllEnemies,
};

UCLASS()
class SPLITSECOND_API AArena : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArena();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectives>> PossibleObjectives;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	

};
