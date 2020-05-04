// This project falls under CC-BY-SA lisence

#include "SplitSecondPlayerController.h"
#include "SplitSecondHUD.h"
#include "Engine/World.h"
#include "../SplitSecondGameState.h"

void ASplitSecondPlayerController::BeginPlay()
{
	Super::BeginPlay();

	Hud = GetHUD<ASplitSecondHUD>();
	if (!ensure(Hud != nullptr)) { return; }
	///TODO Remove Debug Menu
	Hud->ToggleDebugMenu();

	GameState = GetWorld()->GetGameState<ASplitSecondGameState>();
	if (!ensure(GameState != nullptr)) { return; }
}

void ASplitSecondPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!ensure(InputComponent != nullptr)) { return; }

	InputComponent->BindAction(FName("DEBUG_ShowDebugMenu"), EInputEvent::IE_Pressed, this, &ASplitSecondPlayerController::ShowDebugMenu);
	InputComponent->BindAxis(FName("DEBUG_IncreaseSlow"), this, &ASplitSecondPlayerController::IncreaseSlow);
}

void ASplitSecondPlayerController::ShowDebugMenu()
{
	if (!ensure(Hud != nullptr)) { return; }
	Hud->ToggleDebugMenu();
}
void ASplitSecondPlayerController::IncreaseSlow(float Value)
{
	if (!ensure(GameState != nullptr)) { return; }
	GameState->AddToGlobalTimeMultiplier(Value * 0.01);
}