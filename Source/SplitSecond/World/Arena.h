// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Arena.generated.h"

DECLARE_DYNAMIC_DELEGATE(FArenaDelegate);

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
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<EObjectives>> PossibleObjectives;

public:
	AArena();
	virtual void BeginPlay() override;
	FArenaDelegate OnArenaFinished;
	void SpawnActors();

	/* Called when the arenas objective is completed, to give the player the option to continue to the next one*/
	void FinishObjective();
	/* Called when player gives the input to load the next level*/
	void FinishArena();
	
	

};
