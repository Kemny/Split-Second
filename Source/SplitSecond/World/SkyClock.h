// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "SkyClock.generated.h"

UENUM()
enum EClockType
{
	Title,
	Level,
	Highscore,
	Kills

};

UCLASS()
class SPLITSECOND_API ASkyClock : public AStaticMeshActor
{
	GENERATED_BODY()

private:
	ASkyClock();
	void BeginPlay() override;

	UFUNCTION() void ToggleValue();

	bool bShowingValue = false;

	class ASplitSecondGameMode* GameMode;

protected:
	UPROPERTY(VisibleDefaultsOnly)
	class UTextRenderComponent* TextRender;
	UPROPERTY(EditAnywhere)
	float ToggleTime;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EClockType> ClockType;
	
};
