// This project falls under CC-BY-SA lisence

#include "DebugMenu.h"
#include "Kismet/KismetStringLibrary.h"

void UDebugMenu::NativeConstruct()
{
	Super::NativeConstruct();

}

void UDebugMenu::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	Txt_TimeMultiplier->SetText(FText::FromString("Time Multiplier: " + UKismetStringLibrary::Conv_FloatToString(UGameplayStatics::GetGlobalTimeDilation(GetWorld()))));
}