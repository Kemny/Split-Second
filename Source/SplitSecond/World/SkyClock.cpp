// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "SkyClock.h"
#include "TimerManager.h"
#include "../SplitSecondGameMode.h"
#include "Components/TextRenderComponent.h"

ASkyClock::ASkyClock()
{
	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetupAttachment(GetRootComponent());
}

void ASkyClock::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ASkyClock::ToggleValue, ToggleTime, true, 0);

	GameMode = GetWorld()->GetAuthGameMode<ASplitSecondGameMode>();
	if (!ensure(GameMode != nullptr)) { return; }

	if (ClockType == Title)
	{
		TextRender->SetText(FText::FromString("Split\nSecond"));
	}
}

void ASkyClock::ToggleValue()
{
	if (!ensure(GameMode != nullptr)) { return; }

	if (bShowingValue)
	{
		switch (ClockType)
		{
		case Level:
			TextRender->SetText(FText::FromString(FString::FromInt(GameMode->GetArenaNum())));
			break;
		case Highscore:
			TextRender->SetText(FText::FromString(FString::FromInt(GameMode->GetHighscore())));
			break;
		case Kills:
			TextRender->SetText(FText::FromString(FString::FromInt(GameMode->GetKills())));
			break;
		default:
			break;
		}
		bShowingValue = false;
	}
	else
	{
		switch (ClockType)
		{
		case Level:
			TextRender->SetText(FText::FromString("Level"));
			break;
		case Highscore:
			TextRender->SetText(FText::FromString("Highscore"));
			break;
		case Kills:
			TextRender->SetText(FText::FromString("Kills"));
			break;
		default:
			break;
		}

		bShowingValue = true;
	}
}
