// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "PlayerUI.h"

void UPlayerUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bGameSlowAnimations)
	{
		if (TimeSlowDurationProgress == 0)
		{
			TimeSlowRefreshProgress = FMath::Clamp<float>(TimeSlowRefreshProgress + InDeltaTime / TimeSlowRefreshTime, 0, 1);
			ProgressSlowBar->SetPercent(TimeSlowRefreshProgress);
			if (TimeSlowRefreshProgress == 1)
			{
				bGameSlowAnimations = false;
			}
		}
		else
		{
			TimeSlowDurationProgress = FMath::Clamp<float>(TimeSlowDurationProgress - InDeltaTime / TimeSlowDurationTime, 0, 1);
			ProgressSlowBar->SetPercent(TimeSlowDurationProgress);
		}
	}
	

	txt_WaveTimer->SetText(FText::FromString(FString("next wave:\n") + FString::FromInt(FMath::Abs(WaveTargetTime - GetWorld()->GetTimeSeconds()))));
	txt_SurviveTimer->SetText(FText::FromString(FString("Survive Time:\n") + FString::FromInt(FMath::Abs(SurviveTargetTime - GetWorld()->GetTimeSeconds()))));
}

void UPlayerUI::SetRemainingEnemies(int32 Enemies)
{
	txt_RemainingEnemies->SetText(FText::FromString(FString("Enemies:\n") + FString::FromInt(Enemies)));
	txt_RemainingEnemies->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerUI::SetSurviveTime(float Duration)
{
	SurviveTargetTime = GetWorld()->GetTimeSeconds() + Duration;
	txt_SurviveTimer->SetVisibility(ESlateVisibility::Visible);
}
void UPlayerUI::SetNextWaveTime(float Duration)
{
	WaveTargetTime = GetWorld()->GetTimeSeconds() + Duration;
	txt_WaveTimer->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerUI::ActivateTimeSlow(float Cooldown, float Duration)
{
	TimeSlowRefreshProgress = 0;
	TimeSlowRefreshTime = Cooldown - Duration;

	TimeSlowDurationProgress = 1;
	TimeSlowDurationTime = Duration;

	bGameSlowAnimations = true;
}

void UPlayerUI::HandleArenaFinished()
{
	txt_RemainingEnemies->SetVisibility(ESlateVisibility::Collapsed);
	txt_WaveTimer->SetVisibility(ESlateVisibility::Collapsed);
	txt_SurviveTimer->SetVisibility(ESlateVisibility::Collapsed);
}