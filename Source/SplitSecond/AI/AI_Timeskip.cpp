// This project falls under CC-BY-SA lisence


#include "AI_Timeskip.h"
#include "Misc/App.h"

void AAI_Timeskip::SkipTime(float SkipAmount)
{
	FApp::SetCurrentTime(SkipAmount);
}
