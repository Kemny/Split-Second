// This project falls under CC-BY-SA lisence

#include "ActorSpawnLocationComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Flag.h"
#include "TargetLocation.h"

UActorSpawnLocationComponent::UActorSpawnLocationComponent()
{
	SetVisibility(false);

	ConstructorHelpers::FClassFinder<AFlag> BP_Objective_Flag_Class(TEXT("/Game/Blueprint/World/BP_Flag"));
	if (BP_Objective_Flag_Class.Class) Objective_Flag_Class = BP_Objective_Flag_Class.Class;

	ConstructorHelpers::FClassFinder<AFlag> BP_Objective_FlagTarget_Class(TEXT("/Game/Blueprint/World/BP_FlagTarget"));
	if (BP_Objective_FlagTarget_Class.Class) Objective_FlagTarget_Class = BP_Objective_FlagTarget_Class.Class;

	ConstructorHelpers::FClassFinder<ATargetLocation> BP_Objective_PlayerTarget_Class(TEXT("/Game/Blueprint/World/BP_PlayerTarget"));
	if (BP_Objective_PlayerTarget_Class.Class) Objective_PlayerTarget_Class = BP_Objective_PlayerTarget_Class.Class;
}
