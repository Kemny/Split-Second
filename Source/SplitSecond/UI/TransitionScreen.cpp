// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "TransitionScreen.h"
#include "Kismet/GameplayStatics.h"

void UTransitionScreen::PlayWinAnimation(int32 ArenaNum)
{
	AddToPlayerScreen();
	FInputModeUIOnly InputMode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);

	txt_ArenaSuccess->SetVisibility(ESlateVisibility::Visible);
	txt_ArenaNum->SetText(FText::FromString(FString("Arena ") + FString::FromInt(ArenaNum)));

	FWidgetAnimationDynamicEvent AnimEvent;
	AnimEvent.BindUFunction(this, TEXT("HandleAnimationFinished"));
	BindToAnimationFinished(ResultAnimation, AnimEvent);

	WinAnimation = true;

	PlayAnimation(ResultAnimation);
}
void UTransitionScreen::PlayFailAnimation(int32 ArenaNum)
{
	AddToPlayerScreen();
	FInputModeUIOnly InputMode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);

	txt_ArenaFail->SetVisibility(ESlateVisibility::Visible);
	txt_ArenaNum->SetText(FText::FromString(FString("Arena ") + FString::FromInt(ArenaNum)));

	FWidgetAnimationDynamicEvent AnimEvent;
	AnimEvent.BindUFunction(this, TEXT("HandleAnimationFinished"));
	BindToAnimationFinished(ResultAnimation, AnimEvent);

	WinAnimation = false;

	PlayAnimation(ResultAnimation);
}

void UTransitionScreen::HandleAnimationFinished()
{
	FInputModeGameOnly InputMode;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputMode);
	OnTransitionFinished.ExecuteIfBound();
	RemoveFromParent();
}