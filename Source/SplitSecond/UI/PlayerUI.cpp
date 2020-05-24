// This project falls under CC-BY-SA lisence

#include "PlayerUI.h"

void UPlayerUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	TimeSlowProgress = FMath::Clamp<float>(TimeSlowProgress + TimeSlowProgressSpeed, 0, 1);
	TimeSlowCooldown->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), TimeSlowProgress);

	EnemySlowProgress = FMath::Clamp<float>(EnemySlowProgress + EnemySlowProgressSpeed, 0, 1);
	EnemySlowCooldown->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Progress"), EnemySlowProgress);
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