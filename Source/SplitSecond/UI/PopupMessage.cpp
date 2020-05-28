// This project falls under CC-BY-SA lisence


#include "PopupMessage.h"

UPopupMessage::UPopupMessage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bIsFocusable = true;
}

FReply UPopupMessage::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Handled();
}

FReply UPopupMessage::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == KeyToPress)
	{
		FInputModeGameOnly InputMode;
		GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		OnConditionFufilled.ExecuteIfBound();
		RemoveFromParent();
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UPopupMessage::ShowPopupMessage(const FKey& Key, const FText& Text)
{
	FInputModeUIOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	AddToPlayerScreen();
	SetUserFocus(GetWorld()->GetFirstPlayerController());

	Txt_Middle->SetText(Text);
	KeyToPress = Key;
}
