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

public:
	FORCEINLINE void UpdateHealth(float Health, float MaxHealth) { ProgressHealthBar->SetPercent(Health / MaxHealth); }
	
};
