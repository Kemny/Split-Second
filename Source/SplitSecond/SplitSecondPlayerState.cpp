// This project falls under CC-BY-SA lisence

#include "SplitSecondPlayerState.h"

void ASplitSecondPlayerState::SetDefaultWeapon(EWeapons NewWeapon)
{
	switch (NewWeapon)
	{
	case Pistol:
		CurrentStats = FPistolDefault();
		break;
	case Shotgun:
		CurrentStats = FShotgunDefault();
		break;
	case Bow:
		CurrentStats = FBowDefault();
		break;
	default:
		break;
	}
}
