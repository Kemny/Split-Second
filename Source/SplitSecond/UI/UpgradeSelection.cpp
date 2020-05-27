// This project falls under CC-BY-SA lisence

#include "UpgradeSelection.h"
#include "Kismet/KismetStringLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UUpgradeSelection::ShowUpgradeSelection(FUpgrades* CurrentUpgrades, const EWeapons& CurrentWeapon, const bool& bBossUpgrades)
{
	PlayerUpgrades = CurrentUpgrades;
	if (!ensure(PlayerUpgrades != nullptr)) { return; }

	PlayerCurrentWeapon = CurrentWeapon;

	FInputModeUIOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	AddToPlayerScreen();
	SetUserFocus(GetWorld()->GetFirstPlayerController());

	if (!ensure(HealSound != nullptr)) { return; }
	UGameplayStatics::PlaySound2D(GetWorld(), HealSound);

	if (bBossUpgrades)
	{
		TryToCreateBossUpgrade(0);
		TryToCreateBossUpgrade(1);
		TryToCreateBossUpgrade(2);
	}
	else
	{
		TryToCreateRandomUpgrade(0);
		TryToCreateRandomUpgrade(1);
		TryToCreateRandomUpgrade(2);
		TryToCreateRandomUpgrade(3);
		TryToCreateRandomUpgrade(4);
	}

	txt_Homing->SetText(BoolToText(PlayerUpgrades->bIsHoming));
	txt_Piercing->SetText(BoolToText(PlayerUpgrades->bIsPiercing));
	txt_Bouncing->SetText(BoolToText(PlayerUpgrades->bIsBouncing));
	txt_Explosive->SetText(BoolToText(PlayerUpgrades->bExplodingBullets));
	txt_ExtraLife->SetText(BoolToText(PlayerUpgrades->bHasExtraLife));
	txt_ThrowableGun->SetText(BoolToText(PlayerUpgrades->bCanThrowGun));

	txt_Health->SetText(FText::FromString(FloatToString(PlayerUpgrades->Health, 0) + FString(" / ") + FloatToString(PlayerUpgrades->MaxHealth, 0)));
	txt_Damage->SetText(FloatToText(PlayerUpgrades->Damage, 1));
	txt_FireRate->SetText(FloatToText(PlayerUpgrades->FireRate, 1));
	txt_BulletSpeed->SetText(FloatToText(PlayerUpgrades->ProjectileSpeed, 0));

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
		txt_ShotgunBulletSpread->SetText(FText::FromString(FloatToString(100 * PlayerUpgrades->BulletSpread / PlayerUpgrades->BulletSpreadDefault, 1) + "%"));
		break;
	case Bow:
		txt_FireRateName->SetVisibility(ESlateVisibility::Hidden);
		txt_FireRate->SetVisibility(ESlateVisibility::Hidden);
		Name_BowSpecificBox->SetVisibility(ESlateVisibility::Visible);
		Upgrades_BowSpecificBox->SetVisibility(ESlateVisibility::Visible);
		txt_BowDrawSpeed->SetText(FloatToText(PlayerUpgrades->BowDrawSpeed, 3));
		break;
	default:
		UE_LOG(LogTemp, Error, TEXT("Unknown gun enum placed in UpgradeSelection Widget"));
		break;
	}

}

void UUpgradeSelection::SetBossTextUpgradeValue(UTextBlock* TextToSet, EBosssUpgrades TypeToApply)
{
	switch (TypeToApply)
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
void UUpgradeSelection::SetArenaTextUpgradeValue(UTextBlock* TextToSet, EArenaUpgrades TypeToApply)
{
	switch (TypeToApply)
	{
	case EArenaUpgrades::Damage:
		TextToSet->SetText(FText::FromString("Damage\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(TypeToApply), 1)));
		break;
	case EArenaUpgrades::ProjectileSpeed:
		TextToSet->SetText(FText::FromString("Bullet\nSpeed\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(TypeToApply), 0)));
		break;
	case EArenaUpgrades::Health:
		TextToSet->SetText(FText::FromString("Health\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(TypeToApply), 0)));
		break;
	case EArenaUpgrades::FireRate:
		TextToSet->SetText(FText::FromString("Fire\nRate\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(TypeToApply), 1)));
		break;
	case EArenaUpgrades::MaxJumps:
		TextToSet->SetText(FText::FromString("Jumps\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(TypeToApply), 0)));
		break;
	case EArenaUpgrades::BulletNum:
		TextToSet->SetText(FText::FromString("Bullet\nNum\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(TypeToApply), 0)));
		break;
	case EArenaUpgrades::BulletSpread:
		if (!ensure(PlayerUpgrades != nullptr)) { return; }
		TextToSet->SetText(FText::FromString("Bullet\nSpread\n" + FloatToString(100* (*ArenaUpgradeValues.Find(TypeToApply)) / PlayerUpgrades->BulletSpreadDefault, 1) + "%"));
		break;
	case EArenaUpgrades::BowDrawSpeed:
		TextToSet->SetText(FText::FromString("Bow Draw\nSpeed\n" + FString("+") + FloatToString(*ArenaUpgradeValues.Find(TypeToApply), 3)));
		break;
	default:
		break;
	}
}

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

void UUpgradeSelection::TryToCreateBossUpgrade(int32 index)
{
	EBosssUpgrades Type = EBosssUpgrades::NONE;
	if (CreateRandomBossUpgrade(index, Type))
	{
		switch (index)
		{
		case 0:
			Btn_Upgrade1->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectBossUpgradeOne);
			SetBossTextUpgradeValue(txt_UpgradeName1, Type);
			break;
		case 1:
			Btn_Upgrade2->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectBossUpgradeTwo);
			SetBossTextUpgradeValue(txt_UpgradeName2, Type);
			break;
		case 2:
			Btn_Upgrade3->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectBossUpgradeThree);
			SetBossTextUpgradeValue(txt_UpgradeName3, Type);
			break;
		default:
			break;
		}
	}
	else
	{
		TryToCreateRandomUpgrade(index);
	}
}
void UUpgradeSelection::TryToCreateRandomUpgrade(int32 index)
{
	EArenaUpgrades Type = EArenaUpgrades::NONE;

	if (CreateRandomUpgrade(index, Type))
	{
		switch (index)
		{
		case 0:
			SetArenaTextUpgradeValue(txt_UpgradeName1, Type);
			Btn_Upgrade1->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectArenaUpgradeOne);
			break;
		case 1:
			SetArenaTextUpgradeValue(txt_UpgradeName2, Type);
			Btn_Upgrade2->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectArenaUpgradeTwo);
			break;
		case 2:
			SetArenaTextUpgradeValue(txt_UpgradeName3, Type);
			Btn_Upgrade3->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectArenaUpgradeThree);
			break;
		case 3:
			SetArenaTextUpgradeValue(txt_UpgradeName4, Type);
			Btn_Upgrade4->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectArenaUpgradeFour);
			VB_Upgrade4->SetVisibility(ESlateVisibility::Visible);
			break;
		case 4:
			SetArenaTextUpgradeValue(txt_UpgradeName5, Type);
			Btn_Upgrade5->OnClicked.AddUniqueDynamic(this, &UUpgradeSelection::SelectArenaUpgradeFive);
			VB_Upgrade5->SetVisibility(ESlateVisibility::Visible);
			break;
		default:
			break;
		}
	}
	else
	{
		switch (index)
		{
		case 0:
			VB_Upgrade1->SetVisibility(ESlateVisibility::Collapsed);
			break;
		case 1:
			VB_Upgrade2->SetVisibility(ESlateVisibility::Collapsed);
			break;
		case 2:
			VB_Upgrade3->SetVisibility(ESlateVisibility::Collapsed);
			break;
		case 3:
			VB_Upgrade4->SetVisibility(ESlateVisibility::Collapsed);
			break;
		case 4:
			VB_Upgrade5->SetVisibility(ESlateVisibility::Collapsed);
			break;
		default:
			break;
		}
	}

	if (VB_Upgrade1->GetVisibility() == ESlateVisibility::Collapsed && VB_Upgrade2->GetVisibility() == ESlateVisibility::Collapsed && VB_Upgrade3->GetVisibility() == ESlateVisibility::Collapsed && VB_Upgrade4->GetVisibility() == ESlateVisibility::Collapsed && VB_Upgrade5->GetVisibility() == ESlateVisibility::Collapsed)
	{
		ApplyArenaUpgrade(EArenaUpgrades::NONE);
	}
}

bool UUpgradeSelection::CreateRandomUpgrade(int32 index, EArenaUpgrades& OutType)
{
	///This might be THE worst way of doing this
	float Value = 69420;
	int32 AttemptNum = 0;

	for (AttemptNum; AttemptNum < MAX_ATTEMPTS; AttemptNum++)
	{
		switch (FMath::RandRange(0, 7))
		{
		case 0:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::Health))
			{
				auto DigitMultiplier = FMath::Pow(10, FoundValue->MaxDigits);
				int32 HighRandom = FMath::RandRange(FoundValue->MinValue * DigitMultiplier, FoundValue->MaxValue * DigitMultiplier);
				Value = (float)HighRandom / DigitMultiplier;
				OutType = EArenaUpgrades::Health;
			}
			break;
		case 1:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::Damage))
			{
				auto DigitMultiplier = FMath::Pow(10, FoundValue->MaxDigits);
				int32 HighRandom = FMath::RandRange(FoundValue->MinValue * DigitMultiplier, FoundValue->MaxValue * DigitMultiplier);
				Value = (float)HighRandom / DigitMultiplier;
				OutType = EArenaUpgrades::Damage;
			}
			break;
		case 2:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::ProjectileSpeed))
			{
				auto DigitMultiplier = FMath::Pow(10, FoundValue->MaxDigits);
				int32 HighRandom = FMath::RandRange(FoundValue->MinValue * DigitMultiplier, FoundValue->MaxValue * DigitMultiplier);
				Value = (float)HighRandom / DigitMultiplier;
				OutType = EArenaUpgrades::ProjectileSpeed;
			}
			break;
		case 3:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::FireRate))
			{
				if (PlayerCurrentWeapon != EWeapons::Bow)
				{
					auto DigitMultiplier = FMath::Pow(10, FoundValue->MaxDigits);
					int32 HighRandom = FMath::RandRange(FoundValue->MinValue * DigitMultiplier, FoundValue->MaxValue * DigitMultiplier);
					Value = (float)HighRandom / DigitMultiplier;
					OutType = EArenaUpgrades::FireRate;
				}
			}
			break;
		case 4:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::MaxJumps))
			{
				if (PlayerCurrentWeapon == EWeapons::Pistol)
				{
					auto DigitMultiplier = FMath::Pow(10, FoundValue->MaxDigits);
					int32 HighRandom = FMath::RandRange(FoundValue->MinValue * DigitMultiplier, FoundValue->MaxValue * DigitMultiplier);
					Value = (float)HighRandom / DigitMultiplier;
					OutType = EArenaUpgrades::MaxJumps;
				}
			}
			break;
		case 5:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::BulletNum))
			{
				if (PlayerCurrentWeapon == EWeapons::Shotgun && PlayerUpgrades->BulletSpread > 0)
				{
					auto DigitMultiplier = FMath::Pow(10, FoundValue->MaxDigits);
					int32 HighRandom = FMath::RandRange(FoundValue->MinValue * DigitMultiplier, FoundValue->MaxValue * DigitMultiplier);
					Value = (float)HighRandom / DigitMultiplier;
					OutType = EArenaUpgrades::BulletNum;
				}
			}
			break;
		case 6:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::BulletSpread))
			{
				if (PlayerCurrentWeapon == EWeapons::Shotgun)
				{
					auto DigitMultiplier = FMath::Pow(10, FoundValue->MaxDigits);
					int32 HighRandom = FMath::RandRange(FoundValue->MinValue * DigitMultiplier, FoundValue->MaxValue * DigitMultiplier);
					Value = (float)HighRandom / DigitMultiplier;
					OutType = EArenaUpgrades::BulletSpread;
				}
			}
			break;
		case 7:
			if (auto FoundValue = UpgradeValues.Find(EArenaUpgrades::BowDrawSpeed))
			{
				if (PlayerCurrentWeapon == EWeapons::Bow)
				{
					auto DigitMultiplier = FMath::Pow(10, FoundValue->MaxDigits);
					int32 HighRandom = FMath::RandRange(FoundValue->MinValue * DigitMultiplier, FoundValue->MaxValue * DigitMultiplier);
					Value = (float)HighRandom / DigitMultiplier;
					OutType = EArenaUpgrades::BowDrawSpeed;
				}
			}
			break;
		default:
			break;
		}
		if (OutType != EArenaUpgrades::NONE && !ArenaUpgradeValues.Find(OutType))
		{
			break;
		}
	}

	if (AttemptNum >= MAX_ATTEMPTS)
	{
		return false;
	}
	else
	{
		switch (index)
		{
		case 0:
			OneUpgrade = OutType;
			break;
		case 1:
			TwoUpgrade = OutType;
			break;
		case 2:
			ThreeUpgrade = OutType;
			break;
		case 3:
			FourUpgrade = OutType;
			break;
		case 4:
			FiveUpgrade = OutType;
			break;
		default:
			break;
		}

		ArenaUpgradeValues.Add(OutType, Value);
		return true;
	}

}
bool UUpgradeSelection::CreateRandomBossUpgrade(int32 index, EBosssUpgrades& OutType)
{
	if (!ensure(PlayerUpgrades != nullptr)) { return false; }

	///This might be THE worst way of doing this
	int32 AttemptNum = 0;

	for (AttemptNum; AttemptNum < MAX_ATTEMPTS; AttemptNum++)
	{
		switch (FMath::RandRange(0, 3))
		{
		case 0:
			if (!PlayerUpgrades->bIsPiercing)
			{
				OutType = EBosssUpgrades::IsPiercing;
			}
			break;
		case 1:
			if (!PlayerUpgrades->bIsBouncing)
			{
				OutType = EBosssUpgrades::IsBouncing;
			}
			break;
		case 2:
			if (!PlayerUpgrades->bExplodingBullets)
			{
				OutType = EBosssUpgrades::ExplodingBullets;
			}
			break;
		case 3:
			if (!PlayerUpgrades->bHasExtraLife)
			{
				OutType = EBosssUpgrades::ExtraLife;
			}
			break;
		case 4:
			if (!PlayerUpgrades->bIsHoming)
			{
				OutType = EBosssUpgrades::IsHoming;
			}
			break;
		case 5:
			if (!PlayerUpgrades->bCanThrowGun)
			{
				OutType = EBosssUpgrades::CanThrowGun;
			}
			break;
		default:
			break;
		}

		if (OutType != EBosssUpgrades::NONE && !BossUpgradeValues.Find(OutType))
		{
			break;
		}
	}

	if (AttemptNum >= MAX_ATTEMPTS)
	{
		return false;
	}
	else
	{
		switch (index)
		{
		case 0:
			OneUpgradeBoss = OutType;
			break;
		case 1:
			TwoUpgradeBoss = OutType;
			break;
		case 2:
			ThreeUpgradeBoss = OutType;
			break;
		default:
			break;
		}

		BossUpgradeValues.Add(OutType, true);
		return true;
	}
}

#pragma region Button Delegates
#pragma region Arena
void UUpgradeSelection::SelectArenaUpgradeOne()
{
	ApplyArenaUpgrade(OneUpgrade);
}
void UUpgradeSelection::SelectArenaUpgradeTwo()
{
	ApplyArenaUpgrade(TwoUpgrade);
}
void UUpgradeSelection::SelectArenaUpgradeThree()
{
	ApplyArenaUpgrade(ThreeUpgrade);
}
void UUpgradeSelection::SelectArenaUpgradeFour()
{
	ApplyArenaUpgrade(FourUpgrade);
}
void UUpgradeSelection::SelectArenaUpgradeFive()
{
	ApplyArenaUpgrade(FiveUpgrade);
}
void UUpgradeSelection::ApplyArenaUpgrade(EArenaUpgrades TypeToApply)
{
	switch (TypeToApply)
	{
	case EArenaUpgrades::Damage:
		PlayerUpgrades->Damage += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	case EArenaUpgrades::Health:
		PlayerUpgrades->MaxHealth += *ArenaUpgradeValues.Find(TypeToApply);
		PlayerUpgrades->Health += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	case EArenaUpgrades::ProjectileSpeed:
		PlayerUpgrades->ProjectileSpeed += *ArenaUpgradeValues.Find(TypeToApply);
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
		PlayerUpgrades->BulletSpread += FMath::Clamp<float>(*ArenaUpgradeValues.Find(TypeToApply), 0, PlayerUpgrades->BulletSpreadDefault);
		break;
	case EArenaUpgrades::BowDrawSpeed:
		PlayerUpgrades->BowDrawSpeed += *ArenaUpgradeValues.Find(TypeToApply);
		break;
	default:
		break;
	}
	
	FinishSelection();
}
#pragma endregion
#pragma region Boss
void UUpgradeSelection::SelectBossUpgradeOne()
{
	ApplyBossUpgrade(OneUpgradeBoss);
}
void UUpgradeSelection::SelectBossUpgradeTwo()
{
	ApplyBossUpgrade(TwoUpgradeBoss);
}
void UUpgradeSelection::SelectBossUpgradeThree()
{
	ApplyBossUpgrade(ThreeUpgradeBoss);
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

	FinishSelection();
}
#pragma endregion
#pragma endregion

void UUpgradeSelection::FinishSelection()
{
	OnUpgradeSelected.ExecuteIfBound();
	FInputModeGameOnly InputMode;
	GetWorld()->GetFirstPlayerController()->SetInputMode(InputMode);
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

	if (!ensure(SelectUpgradeSound != nullptr)) { return; }
	UGameplayStatics::PlaySound2D(GetWorld(), SelectUpgradeSound);

	RemoveFromParent();
}
