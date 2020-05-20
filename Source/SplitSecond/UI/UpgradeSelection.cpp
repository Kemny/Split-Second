// This project falls under CC-BY-SA lisence

#include "UpgradeSelection.h"
#include "Kismet/KismetStringLibrary.h"

void UUpgradeSelection::ShowUpgradeSelection(FUpgrades& CurrentUpgrades, const EWeapons& CurrentWeapon, const bool& bBossUpgrades, const float& CurrentHealth, const float& MaxHealth)
{
	Btn_Upgrade1->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectUpgradeOne);
	Btn_Upgrade2->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectUpgradeTwo);
	Btn_Upgrade3->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectUpgradeThree);

	txt_UpgradeName1;
	txt_UpgradeName2;
	txt_UpgradeName3;

	if (bBossUpgrades)
	{
		VB_Upgrade4;
		VB_Upgrade5;

		Btn_Upgrade4->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectUpgradeFour);
		Btn_Upgrade5->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectUpgradeFive);

		txt_UpgradeName4;
		txt_UpgradeName5;
	}

	txt_Homing->SetText(BoolToText(CurrentUpgrades.bIsHoming));
	txt_Piercing->SetText(BoolToText(CurrentUpgrades.bIsPiercing));
	txt_Bouncing->SetText(BoolToText(CurrentUpgrades.bIsBouncing));
	txt_Explosive->SetText(BoolToText(CurrentUpgrades.bExplodingBullets));
	txt_ExtraLife->SetText(BoolToText(CurrentUpgrades.bHasExtraLife));
	txt_ThrowableGun->SetText(BoolToText(CurrentUpgrades.bCanThrowGun));

	txt_Health->SetText(FText::FromString(FloatToString(CurrentHealth, 0) + FString(" / ") + FloatToString(MaxHealth, 0)));
	txt_Damage->SetText(FloatToText(CurrentUpgrades.Damage, 1));
	txt_FireRate->SetText(FloatToText(CurrentUpgrades.FireRate, 1));
	txt_BulletSpeed->SetText(FloatToText(CurrentUpgrades.ProjectileSpeed, 2));
	txt_ReloadSpeed->SetText(FloatToText(CurrentUpgrades.ReloadSpeed, 1));
	txt_MaxAmmo->SetText(IntToText(CurrentUpgrades.MaxAmmo));

	switch (CurrentWeapon)
	{
	case Pistol:
		Name_PistolSpecificBox->SetVisibility(ESlateVisibility::Visible);
		Upgrades_PistolSpecificBox->SetVisibility(ESlateVisibility::Visible);
		txt_JumpNum->SetText(IntToText(CurrentUpgrades.MaxJumps));
		break;
	case Shotgun:
		Name_ShotgunSpecificBox->SetVisibility(ESlateVisibility::Visible);
		Upgrades_ShotgunSpecificBox->SetVisibility(ESlateVisibility::Visible);
		txt_ShotgunBulletNum->SetText(IntToText(CurrentUpgrades.BulletNum));
		txt_ShotgunBulletSpread->SetText(FloatToText(CurrentUpgrades.BulletSpread, 1));
		break;
	case Bow:
		Name_BowSpecificBox->SetVisibility(ESlateVisibility::Visible);
		Upgrades_BowSpecificBox->SetVisibility(ESlateVisibility::Visible);
		txt_BowDrawSpeed->SetText(FloatToText(CurrentUpgrades.BowDrawSpeed, 3));
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Unknown gun enum placed in UpgradeSelection Widget"));
		break;
	}

}

void UUpgradeSelection::SelectUpgradeOne()
{

}
void UUpgradeSelection::SelectUpgradeTwo()
{

}
void UUpgradeSelection::SelectUpgradeThree()
{

}
void UUpgradeSelection::SelectUpgradeFour()
{

}
void UUpgradeSelection::SelectUpgradeFive()
{

}

FText UUpgradeSelection::BoolToText(const bool& BoolToConvert)
{
	if (BoolToConvert)
	{
		return FText::FromString("True");
	}
	else
	{
		return FText::FromString("False");
	}
}
FText UUpgradeSelection::FloatToText(const float& Value, const int32& FractalDigits)
{
	return FText::FromString(FString::SanitizeFloat(Value, FractalDigits));
}
FString UUpgradeSelection::FloatToString(const float& Value, const int32& FractalDigits)
{
	return FString::SanitizeFloat(Value, FractalDigits);
}
FText UUpgradeSelection::IntToText(const int32& Value)
{
	return FText::FromString(FString::FromInt(Value));
}
FText UUpgradeSelection::IntToTextWithPlus(const int32& Value)
{
	if (Value > 0)
	{
		return FText::FromString(FString("+") += FString::FromInt(Value));
	}
	else
	{
		return FText::FromString(FString::FromInt(Value));
	}
}
