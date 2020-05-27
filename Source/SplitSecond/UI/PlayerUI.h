// This project falls under CC-BY-SA lisence

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
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* ProgressHealthBar;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_Objective;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_WaveTimer;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_SurviveTimer;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_RemainingEnemies;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* TimeSlowCooldown;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	float TimeSlowProgress = 1;
	float TimeSlowProgressSpeed = 1;

	float SurviveTargetTime = 0;
	float WaveTargetTime = 0;

public:
	FORCEINLINE void SetObjectiveName(FString ObjectiveName) { txt_Objective->SetText(FText::FromString(FString("Current Objective:\n" + ObjectiveName))); }

	void ActivateTimeSlow(float Cooldown);
	
	void SetSurviveTime(float Duration);
	void SetNextWaveTime(float Duration);
	void SetRemainingEnemies(int32 Enemies);

	void HandleArenaFinished();
};
