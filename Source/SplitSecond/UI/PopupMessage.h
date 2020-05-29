// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UMG.h"

#include "PopupMessage.generated.h"

DECLARE_DYNAMIC_DELEGATE(FPopupMessageDelegate);

UCLASS()
class SPLITSECOND_API UPopupMessage : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* Txt_Middle;

public:
	UPopupMessage(const FObjectInitializer& ObjectInitializer);

	FPopupMessageDelegate OnConditionFufilled;

	FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	void ShowPopupMessage(const FKey& Key, const FText& Text);

private:
	FKey KeyToPress;
};
