// This project falls under CC-BY-SA lisence

#include "ActorSpawnLocationComponent.h"
#include "UObject/ConstructorHelpers.h"

UActorSpawnLocationComponent::UActorSpawnLocationComponent()
{
	bVisible = false;

	ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Charger_Class(TEXT("/Game/Blueprint/NPC/BP_Charger"));
	if (BP_Enemy_Charger_Class.Class) Enemy_Charger_Class = BP_Enemy_Charger_Class.Class;

	ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Healer_Class(TEXT("/Game/Blueprint/NPC/BP_Healer"));
	if (BP_Enemy_Healer_Class.Class) Enemy_Healer_Class = BP_Enemy_Healer_Class.Class;

	ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Shooter_Class(TEXT("/Game/Blueprint/NPC/BP_Shooter"));
	if (BP_Enemy_Shooter_Class.Class) Enemy_Shooter_Class = BP_Enemy_Shooter_Class.Class;

	ConstructorHelpers::FClassFinder<AActor> BP_Enemy_Turret_Class(TEXT("/Game/Blueprint/NPC/BP_Turret"));
	if (BP_Enemy_Turret_Class.Class) Enemy_Turret_Class = BP_Enemy_Turret_Class.Class;

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
	case Enemy_Charger:
		return Enemy_Charger_Class;
		break;
	case Enemy_Healer:
		return Enemy_Healer_Class;
		break;
	case Enemy_Shooter:
		return Enemy_Shooter_Class;
		break;
	case Enemy_Turret:
		return Enemy_Turret_Class;
		break;
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

