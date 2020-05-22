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
	UImage* TimeSlowCooldown;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* EnemySlowCooldown;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	float TimeSlowProgress = 1;
	float TimeSlowProgressSpeed = 1;

	float EnemySlowProgress = 1;
	float EnemySlowProgressSpeed = 1;

public:
	FORCEINLINE void UpdateHealth(float Health, float MaxHealth) { ProgressHealthBar->SetPercent(Health / MaxHealth); }
	FORCEINLINE void SetObjectiveName(FString ObjectiveName) { txt_Objective->SetText(FText::FromString(FString("Current Objective:\n" + ObjectiveName))); }

	void ActivateTimeSlow(float Cooldown);
	void ActivateEnemySlow(float Cooldown);
	
};
