// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UMG.h"

#include "DebugMenu.generated.h"

UCLASS()
class SPLITSECOND_API UDebugMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Txt_TimeMultiplier;

private:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	class ASplitSecondGameState* GameState;
};
