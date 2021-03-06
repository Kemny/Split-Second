// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UMG.h"

#include "PlayerUI.generated.h"

UCLASS()
class SPLITSECOND_API UPlayerUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressHealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_WaveTimer;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_SurviveTimer;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_RemainingEnemies;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressSlowBar;


private:
	bool bGameSlowAnimations = false;

	float TimeSlowRefreshProgress = 1;
	float TimeSlowRefreshTime = 1;

	float TimeSlowDurationProgress = 1;
	float TimeSlowDurationTime = 1;

	float SurviveTargetTime = 0;
	float WaveTargetTime = 0;

public:
	void ActivateTimeSlow(float Cooldown, float Duration);
	
	void SetSurviveTime(float Duration);
	void SetNextWaveTime(float Duration);
	void SetRemainingEnemies(int32 Enemies);

	void HandleArenaFinished();
};
