// This project falls under CC-BY-SA lisence


#include "PopupMessage.h"

UPopupMessage::UPopupMessage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}

FReply UPopupMessage::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == KeyToPress)
	{
		OnConditionFufilled.ExecuteIfBound();
		RemoveFromParent();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UPopupMessage::ShowPopupMessage(const FKey& Key, const FText& Text)
{
	Txt_Middle->SetText(Text);
	KeyToPress = Key;
}