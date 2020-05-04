// This project falls under CC-BY-SA lisence

#include "DebugMenu.h"
#include "Kismet/KismetStringLibrary.h"
#include "../SplitSecondGameState.h"

void UDebugMenu::NativeConstruct()
{
	Super::NativeConstruct();

	GameState = GetWorld()->GetGameState<ASplitSecondGameState>();
	if (!ensure(GameState != nullptr)) { return; }
}

void UDebugMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!ensure(GameState != nullptr)) { return; }

	Txt_TimeMultiplier->SetText(FText::FromString("Time Multiplier: " + UKismetStringLibrary::Conv_FloatToString(GameState->GetGlobalTimeMultiplier())));
}