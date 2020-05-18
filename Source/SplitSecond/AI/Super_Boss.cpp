// This project falls under CC-BY-SA lisence


#include "Super_Boss.h"
#include "Kismet/GameplayStatics.h"

void ASuper_Boss::ResetTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1);
}
