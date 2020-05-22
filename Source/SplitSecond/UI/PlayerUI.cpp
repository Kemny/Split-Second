// This project falls under CC-BY-SA lisence

#include "PlayerUI.h"

void UPlayerUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (TimeSlowProgress <= 1)
	{
		TimeSlowProgress += TimeSlowProgressSpeed * 0.1;
		TimeSlowProgress = FMath::Clamp<float>(TimeSlowProgress, 0, TimeSlowProgress + TimeSlowProgressSpeed);
		TimeSlowCooldown->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), TimeSlowProgress);

	}
	if (EnemySlowProgress <= 1)
	{
		EnemySlowProgress += EnemySlowProgressSpeed;
		EnemySlowProgress = FMath::Clamp<float>(EnemySlowProgress, 0, 1);
		EnemySlowCooldown->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), EnemySlowProgress);
	}
}

void UPlayerUI::ActivateTimeSlow(float Cooldown)
{
	TimeSlowProgress = 0;
	//Not exactly accurate
	TimeSlowProgressSpeed = Cooldown * FApp::GetDeltaTime() * 0.1;
}
void UPlayerUI::ActivateEnemySlow(float Cooldown)
{
	EnemySlowProgress = 0;
	//Not exactly accurate
	EnemySlowProgressSpeed = Cooldown * FApp::GetDeltaTime() * 0.1;
}