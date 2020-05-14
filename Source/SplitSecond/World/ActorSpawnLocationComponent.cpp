// This project falls under CC-BY-SA lisence

#include "ActorSpawnLocationComponent.h"
#include "UObject/ConstructorHelpers.h"

UActorSpawnLocationComponent::UActorSpawnLocationComponent()
{
	SetVisibility(false);

	ConstructorHelpers::FClassFinder<AActor> BP_Objective_Flag_Class(TEXT("/Game/Blueprint/World/BP_Flag"));
	if (BP_Objective_Flag_Class.Class) Objective_Flag_Class = BP_Objective_Flag_Class.Class;

	ConstructorHelpers::FClassFinder<AActor> BP_Objective_FlagTarget_Class(TEXT("/Game/Blueprint/World/BP_FlagTarget"));
	if (BP_Objective_FlagTarget_Class.Class) Objective_FlagTarget_Class = BP_Objective_FlagTarget_Class.Class;

	ConstructorHelpers::FClassFinder<AActor> BP_Objective_PlayerTarget_Class(TEXT("/Game/Blueprint/World/BP_PlayerTarget"));
	if (BP_Objective_PlayerTarget_Class.Class) Objective_PlayerTarget_Class = BP_Objective_PlayerTarget_Class.Class;
}

TSubclassOf<AActor> UActorSpawnLocationComponent::GetCurrentTypeClass() const
{
	switch (SpawnType)
	{
	case Objective_Flag:
		return Objective_Flag_Class;
		break;
	case Objective_FlagTarget:
		return Objective_FlagTarget_Class;
		break;
	case Objective_PlayerTarget:
		return Objective_PlayerTarget_Class;
		break;
	case Player_Location:
		return TSubclassOf<AActor>();
		break;
	default:
		return TSubclassOf<AActor>();
		break;
	}
}

