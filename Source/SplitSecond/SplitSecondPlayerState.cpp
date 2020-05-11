// This project falls under CC-BY-SA lisence

#include "SplitSecondPlayerState.h"

void ASplitSecondPlayerState::SetDefaultWeapon(EWeapons NewWeapon)
{
	switch (NewWeapon)
	{
	case Pistol:
		CurrentUpgrades = FPistolDefault();
		break;
	case Shotgun:
		CurrentUpgrades = FShotgunDefault();
		break;
	case Bow:
		CurrentUpgrades = FBowDefault();
		break;
	default:
		break;
	}
}
