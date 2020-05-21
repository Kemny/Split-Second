// This project falls under CC-BY-SA lisence

#include "UpgradeSelection.h"
#include "Kismet/KismetStringLibrary.h"
#include "../Health/HealthComponent.h"

void UUpgradeSelection::ShowUpgradeSelection(FUpgrades* CurrentUpgrades, const EWeapons& CurrentWeapon, const bool& bBossUpgrades, UHealthComponent* HealthComponent)
{
	PlayerHealthComponent = HealthComponent;
	if (!ensure(PlayerHealthComponent != nullptr)) { return; }

	PlayerUpgrades = CurrentUpgrades;
	if (!ensure(PlayerUpgrades != nullptr)) { return; }

	PlayerCurrentWeapon = CurrentWeapon;

	FInputModeGameAndUI InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	AddToPlayerScreen();
	SetUserFocus(GetWorld()->GetFirstPlayerController());


	if (bBossUpgrades)
	{
		CreateRandomBossUpgrade();
		CreateRandomBossUpgrade();
		CreateRandomBossUpgrade();

		Btn_Upgrade1->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectBossUpgradeOne);
		Btn_Upgrade2->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectBossUpgradeTwo);
		Btn_Upgrade3->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectBossUpgradeThree);

		SetBossTextUpgradeValue(txt_UpgradeName1, 0);
		SetBossTextUpgradeValue(txt_UpgradeName2, 1);
		SetBossTextUpgradeValue(txt_UpgradeName3, 2);
	}
	else
	{
		VB_Upgrade4->SetVisibility(ESlateVisibility::Visible);
		VB_Upgrade5->SetVisibility(ESlateVisibility::Visible);

		Btn_Upgrade1->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectArenaUpgradeOne);
		Btn_Upgrade2->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectArenaUpgradeTwo);
		Btn_Upgrade3->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectArenaUpgradeThree);
		Btn_Upgrade4->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectArenaUpgradeFour);
		Btn_Upgrade5->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectArenaUpgradeFive);

		CreateRandomUpgrade();
		CreateRandomUpgrade();
		CreateRandomUpgrade();
		CreateRandomUpgrade();
		CreateRandomUpgrade();

		SetArenaTextUpgradeValue(txt_UpgradeName1, 0);
		SetArenaTextUpgradeValue(txt_UpgradeName2, 1);
		SetArenaTextUpgradeValue(txt_UpgradeName3, 2);
		SetArenaTextUpgradeValue(txt_UpgradeName4, 3);
		SetArenaTextUpgradeValue(txt_UpgradeName5, 4);
	}

	txt_Homing->SetText(BoolToText(PlayerUpgrades->bIsHoming));
	txt_Piercing->SetText(BoolToText(PlayerUpgrades->bIsPiercing));
	txt_Bouncing->SetText(BoolToText(PlayerUpgrades->bIsBouncing));
	txt_Explosive->SetText(BoolToText(PlayerUpgrades->bExplodingBullets));
	txt_ExtraLife->SetText(BoolToText(PlayerUpgrades->bHasExtraLife));
	txt_ThrowableGun->SetText(BoolToText(PlayerUpgrades->bCanThrowGun));

	txt_Health->SetText(FText::FromString(FloatToString(PlayerHealthComponent->GetHealth(), 0) + FString(" / ") + FloatToString(PlayerHealthComponent->GetMaxHealth(), 0)));
	txt_Damage->SetText(FloatToText(PlayerUpgrades->Damage, 1));
	txt_FireRate->SetText(FloatToText(PlayerUpgrades->FireRate, 1));
	txt_BulletSpeed->SetText(FloatToText(PlayerUpgrades->ProjectileSpeed, 2));
	txt_ReloadSpeed->SetText(FloatToText(PlayerUpgrades->ReloadSpeed, 1));
	txt_MaxAmmo->SetText(IntToText(PlayerUpgrades->MaxAmmo));

	switch (CurrentWeapon)
	{
	case Pistol:
		Name_PistolSpecificBox->SetVisibility(ESlateVisibility::Visible);
		Upgrades_PistolSpecificBox->SetVisibility(ESlateVisibility::Visible);
		txt_JumpNum->SetText(IntToText(PlayerUpgrades->MaxJumps));
		break;
	case Shotgun:
		Name_ShotgunSpecificBox->SetVisibility(ESlateVisibility::Visible);
		Upgrades_ShotgunSpecificBox->SetVisibility(ESlateVisibility::Visible);
		txt_ShotgunBulletNum->SetText(IntToText(PlayerUpgrades->BulletNum));
		txt_ShotgunBulletSpread->SetText(FloatToText(PlayerUpgrades->BulletSpread, 1));
		break;
	case Bow:
		Name_BowSpecificBox->SetVisibility(ESlateVisibility::Visible);
		Upgrades_BowSpecificBox->SetVisibility(ESlateVisibility::Visible);
		txt_BowDrawSpeed->SetText(FloatToText(PlayerUpgrades->BowDrawSpeed, 3));
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Unknown gun enum placed in UpgradeSelection Widget"));
		break;
	}

}

void UUpgradeSelection::SetBossTextUpgradeValue(UTextBlock* TextToSet, int32 ButtonIndex)
{
	TArray<EBosssUpgrades> Keys;
	BossUpgradeValues.GenerateKeyArray(Keys);

	if (!ensure(Keys.Num() > ButtonIndex)) { return; }
	auto Upgrade = Keys[ButtonIndex];

	switch (Upgrade)
	{
	case EBosssUpgrades::IsPiercing:
		TextToSet->SetText(FText::FromString("Piercing Bullets"));
		break;
	case EBosssUpgrades::IsHoming:
		TextToSet->SetText(FText::FromString("Homing Bullets"));
		break;
	case EBosssUpgrades::IsBouncing:
		TextToSet->SetText(FText::FromString("Bouncing Bullets"));
		break;
	case EBosssUpgrades::ExplodingBullets:
		TextToSet->SetText(FText::FromString("Exploding Bullets"));
		break;
	case EBosssUpgrades::ExtraLife:
		TextToSet->SetText(FText::FromString("Extra Life"));
		break;
	case EBosssUpgrades::CanThrowGun:
		TextToSet->SetText(FText::FromString("Throwable Gun"));
		break;
	default:
		break;
	}
}
void UUpgradeSelection::SetArenaTextUpgradeValue(UTextBlock* TextToSet, int32 ButtonIndex)
{
	TArray<EArenaUpgrades> Keys;
	ArenaUpgradeValues.GenerateKeyArray(Keys);

	if (!ensure(Keys.Num() > ButtonIndex)) { return; }
	auto Upgrade = Keys[ButtonIndex];

	switch (Upgrade)
	{
	case EArenaUpgrades::Ammo:
		TextToSet->SetText(FText::FromString("Ammo\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(Upgrade), 0)));
		break;
	case EArenaUpgrades::Damage:
		TextToSet->SetText(FText::FromString("Damage\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(Upgrade), 1)));
		break;
	case EArenaUpgrades::ProjectileSpeed:
		TextToSet->SetText(FText::FromString("Bullet Speed\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(Upgrade), 1)));
		break;
	case EArenaUpgrades::ReloadSpeed:
		TextToSet->SetText(FText::FromString("Reload Speed\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(Upgrade), 2)));
		break;
	case EArenaUpgrades::FireRate:
		TextToSet->SetText(FText::FromString("Fire Rate\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(Upgrade), 2)));
		break;
	case EArenaUpgrades::MaxJumps:
		TextToSet->SetText(FText::FromString("Jumps\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(Upgrade), 0)));
		break;
	case EArenaUpgrades::BulletNum:
		TextToSet->SetText(FText::FromString("Ammo\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(Upgrade), 0)));
		break;
	case EArenaUpgrades::BulletSpread:
		TextToSet->SetText(FText::FromString("Bullet Spread\n" + FString("-") + FloatToString(*ArenaUpgradeValues.Find(Upgrade), 2)));
		break;
	case EArenaUpgrades::BowDrawSpeed:
		TextToSet->SetText(FText::FromString("Bow Draw Speed\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(Upgrade), 2)));
		break;
	default:
		break;
	}
}

#pragma region Button Delegates
#pragma region Arena
void UUpgradeSelection::SelectArenaUpgradeOne()
{
	TArray<EArenaUpgrades> Keys;
	ArenaUpgradeValues.GenerateKeyArray(Keys);

	if (!ensure(Keys.Num() >= 1)) { return; }

	ApplyArenaUpgrade(Keys[0]);
}
void UUpgradeSelection::SelectArenaUpgradeTwo()
{
	TArray<EArenaUpgrades> Keys;
	ArenaUpgradeValues.GenerateKeyArray(Keys);

	if (!ensure(Keys.Num() >= 2)) { return; }
	ApplyArenaUpgrade(Keys[1]);
}
void UUpgradeSelection::SelectArenaUpgradeThree()
{
	TArray<EArenaUpgrades> Keys;
	ArenaUpgradeValues.GenerateKeyArray(Keys);

	if (!ensure(Keys.Num() >= 3)) { return; }
	ApplyArenaUpgrade(Keys[2]);
}
void UUpgradeSelection::SelectArenaUpgradeFour()
{
	TArray<EArenaUpgrades> Keys;
	ArenaUpgradeValues.GenerateKeyArray(Keys);

	if (!ensure(Keys.Num() >= 4)) { return; }
	ApplyArenaUpgrade(Keys[3]);
}
void UUpgradeSelection::SelectArenaUpgradeFive()
{
	TArray<EArenaUpgrades> Keys;
	ArenaUpgradeValues.GenerateKeyArray(Keys);

	if (!ensure(Keys.Num() >= 5)) { return; }
	ApplyArenaUpgrade(Keys[4]);
}
void UUpgradeSelection::ApplyArenaUpgrade(EArenaUpgrades TypeToApply)
{
	switch (TypeToApply)
	{
	case EArenaUpgrades::Ammo:
		PlayerUpgrades->Ammo += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	case EArenaUpgrades::Damage:
		PlayerUpgrades->Damage += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	case EArenaUpgrades::ProjectileSpeed:
		PlayerUpgrades->ProjectileSpeed += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	case EArenaUpgrades::ReloadSpeed:
		PlayerUpgrades->ReloadSpeed += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	case EArenaUpgrades::FireRate:
		PlayerUpgrades->FireRate += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	case EArenaUpgrades::MaxJumps:
		PlayerUpgrades->MaxJumps += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	case EArenaUpgrades::BulletNum:
		PlayerUpgrades->BulletNum += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	case EArenaUpgrades::BulletSpread:
		PlayerUpgrades->BulletSpread += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	case EArenaUpgrades::BowDrawSpeed:
		PlayerUpgrades->BowDrawSpeed += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	default:
		break;
	}
	OnUpgradeSelected.ExecuteIfBound();
	FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	RemoveFromParent();
}
#pragma endregion
#pragma region Boss
void UUpgradeSelection::SelectBossUpgradeOne()
{
	TArray<EBosssUpgrades> Keys;
	BossUpgradeValues.GenerateKeyArray(Keys);

	if (!ensure(Keys.Num() >= 1)) { return; }

	ApplyBossUpgrade(Keys[0]);
}
void UUpgradeSelection::SelectBossUpgradeTwo()
{
	TArray<EBosssUpgrades> Keys;
	BossUpgradeValues.GenerateKeyArray(Keys);

	if (!ensure(Keys.Num() >= 2)) { return; }

	ApplyBossUpgrade(Keys[1]);
}
void UUpgradeSelection::SelectBossUpgradeThree()
{
	TArray<EBosssUpgrades> Keys;
	BossUpgradeValues.GenerateKeyArray(Keys);

	if (!ensure(Keys.Num() >= 3)) { return; }

	ApplyBossUpgrade(Keys[2]);
}
void UUpgradeSelection::ApplyBossUpgrade(EBosssUpgrades TypeToApply)
{
	if (!ensure(PlayerUpgrades != nullptr)) { return; }

	switch (TypeToApply)
	{
	case EBosssUpgrades::IsPiercing:
		PlayerUpgrades->bIsPiercing = true;
		break;
	case EBosssUpgrades::IsHoming:
		PlayerUpgrades->bIsHoming = true;
		break;
	case EBosssUpgrades::IsBouncing:
		PlayerUpgrades->bIsBouncing = true;
		break;
	case EBosssUpgrades::ExplodingBullets:
		PlayerUpgrades->bExplodingBullets = true;
		break;
	case EBosssUpgrades::ExtraLife:
		PlayerUpgrades->bHasExtraLife = true;
		break;
	case EBosssUpgrades::CanThrowGun:
		PlayerUpgrades->bCanThrowGun = true;
		break;
	default:
		break;
	}
	OnUpgradeSelected.ExecuteIfBound();
	FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	RemoveFromParent();
}
#pragma endregion
#pragma endregion

#pragma region Conversions
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
#pragma endregion

void UUpgradeSelection::CreateRandomUpgrade()
{
	///This might be THE worst way of doing this
	EArenaUpgrades Type = EArenaUpgrades::NONE;
	float Value = 69420;
	int32 AttemptNum = 0;
	do
	{
		++AttemptNum;
		switch (FMath::RandRange(0, 8))
		{
		case 0:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::Ammo))
			{
				Value = FMath::RandRange((int32)FoundValue->MinValue, (int32)FoundValue->MaxValue);
				Type = EArenaUpgrades::Ammo;
			}
			break;
		case 1:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::Damage))
			{
				Value = FMath::RandRange(FoundValue->MinValue, FoundValue->MaxValue);
				Type = EArenaUpgrades::Damage;
			}
			break;
		case 2:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::ProjectileSpeed))
			{
				Value = FMath::RandRange(FoundValue->MinValue, FoundValue->MaxValue);
				Type = EArenaUpgrades::ProjectileSpeed;
			}
			break;
		case 3:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::ReloadSpeed))
			{
				Value = FMath::RandRange(FoundValue->MinValue, FoundValue->MaxValue);
				Type = EArenaUpgrades::ReloadSpeed;
			}
			break;
		case 4:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::FireRate))
			{
				Value = FMath::RandRange(FoundValue->MinValue, FoundValue->MaxValue);
				Type = EArenaUpgrades::FireRate;
			}
			break;
		case 5:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::MaxJumps))
			{
				if (PlayerCurrentWeapon == EWeapons::Pistol)
				{
					Value = FMath::RandRange((int32)FoundValue->MinValue, (int32)FoundValue->MaxValue);
					Type = EArenaUpgrades::MaxJumps;
				}
			}
			break;
		case 6:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::BulletNum))
			{
				if (PlayerCurrentWeapon == EWeapons::Shotgun)
				{
					Value = FMath::RandRange((int32)FoundValue->MinValue, (int32)FoundValue->MaxValue);
					Type = EArenaUpgrades::BulletNum;
				}
			}
			break;
		case 7:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::BulletSpread))
			{
				if (PlayerCurrentWeapon == EWeapons::Shotgun)
				{
					Value = FMath::RandRange((int32)FoundValue->MinValue, (int32)FoundValue->MaxValue);
					Type = EArenaUpgrades::BulletSpread;
				}
			}
			break;
		case 8:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::BowDrawSpeed))
			{
				if (PlayerCurrentWeapon == EWeapons::Bow)
				{
					Value = FMath::RandRange(FoundValue->MinValue, FoundValue->MaxValue);
					Type = EArenaUpgrades::BowDrawSpeed;
				}
			}
			break;
		default:
			break;
		}
	} while (Type == EArenaUpgrades::NONE || ArenaUpgradeValues.Find(Type) || AttemptNum > MAX_ATTEMPTS);

	ArenaUpgradeValues.Add(Type, Value);
}
void UUpgradeSelection::CreateRandomBossUpgrade()
{
	if (!ensure(PlayerUpgrades != nullptr)) { return; }

	///This might be THE worst way of doing this
	EBosssUpgrades Type = EBosssUpgrades::NONE;
	int32 AttemptNum = 0;
	do
	{
		++AttemptNum;
		switch (FMath::RandRange(0, 5))
		{
		case 0:
			if (!PlayerUpgrades->bIsPiercing)
			{
				Type = EBosssUpgrades::IsPiercing;
			}
			break;
		case 1:
			if (!PlayerUpgrades->bIsHoming)
			{
				Type = EBosssUpgrades::IsHoming;
			}
			break;
		case 2:
			if (!PlayerUpgrades->bIsBouncing)
			{
				Type = EBosssUpgrades::IsBouncing;
			}
			break;
		case 3:
			if (!PlayerUpgrades->bExplodingBullets)
			{
				Type = EBosssUpgrades::ExplodingBullets;
			}
			break;
		case 4:
			if (!PlayerUpgrades->bHasExtraLife)
			{
				Type = EBosssUpgrades::ExtraLife;
			}
			break;
		case 5:
			if (!PlayerUpgrades->bCanThrowGun)
			{
				Type = EBosssUpgrades::CanThrowGun;
			}
			break;
		default:
			break;
		}
	} while (Type == EBosssUpgrades::NONE || BossUpgradeValues.Find(Type) || AttemptNum > MAX_ATTEMPTS);

	BossUpgradeValues.Add(Type, true);
}
