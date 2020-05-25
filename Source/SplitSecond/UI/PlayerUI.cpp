// This project falls under CC-BY-SA lisence

#include "PlayerUI.h"

void UPlayerUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TimeSlowProgress = FMath::Clamp<float>(TimeSlowProgress + TimeSlowProgressSpeed, 0, 1);
	TimeSlowCooldown->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), TimeSlowProgress);

	EnemySlowProgress = FMath::Clamp<float>(EnemySlowProgress + EnemySlowProgressSpeed, 0, 1);
	EnemySlowCooldown->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), EnemySlowProgress);

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

void UPlayerUI::ActivateTimeSlow(float Cooldown)
{
	TimeSlowProgress = 0;
	//Not exactly accurate
	TimeSlowProgressSpeed = Cooldown * FApp::GetDeltaTime() * 0.01;
}
void UPlayerUI::ActivateEnemySlow(float Cooldown)
{
	EnemySlowProgress = 0;
	//Not exactly accurate
	EnemySlowProgressSpeed = Cooldown * FApp::GetDeltaTime() * 0.1;
}

void UPlayerUI::HandleArenaFinished()
{
	txt_RemainingEnemies->SetVisibility(ESlateVisibility::Collapsed);
	txt_WaveTimer->SetVisibility(ESlateVisibility::Collapsed);
	txt_SurviveTimer->SetVisibility(ESlateVisibility::Collapsed);
}