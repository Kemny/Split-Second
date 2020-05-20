// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UMG.h"
#include "../Libraries/SplitSecondWeapons.h"

#include "UpgradeSelection.generated.h"

UCLASS()
class SPLITSECOND_API UUpgradeSelection : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void ShowUpgradeSelection(FUpgrades& CurrentUpgrades, const EWeapons& CurrentWeapon, const bool& bBossUpgrades, const float& CurrentHealth, const float& MaxHealth);

protected:
#pragma region Upgrade Selection
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VB_Upgrade1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VB_Upgrade2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VB_Upgrade3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VB_Upgrade4;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* VB_Upgrade5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Upgrade1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Upgrade2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Upgrade3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Upgrade4;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* Btn_Upgrade5;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_UpgradeName1;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_UpgradeName2;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_UpgradeName3;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_UpgradeName4;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_UpgradeName5;
#pragma endregion

#pragma region Player Stats
#pragma region Bools
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_Homing;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_Piercing;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_Bouncing;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_Explosive;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_ExtraLife;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_ThrowableGun;
#pragma endregion
#pragma region Values
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_Health;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_Damage;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_FireRate;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_BulletSpeed;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_ReloadSpeed;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_MaxAmmo;
#pragma endregion
#pragma region Pistol
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* Name_PistolSpecificBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* Upgrades_PistolSpecificBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_JumpNum;
#pragma endregion
#pragma region Shotgun
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* Name_ShotgunSpecificBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* Upgrades_ShotgunSpecificBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_ShotgunBulletNum;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_ShotgunBulletSpread;
#pragma endregion
#pragma region Bow
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* Name_BowSpecificBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* Upgrades_BowSpecificBox;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_BowDrawSpeed;
#pragma endregion
#pragma endregion

private:
	FText BoolToText(const bool& BoolToConvert);
	FText FloatToText(const float& Value, const int32& FractalDigits);
	FString FloatToString(const float& Value, const int32& FractalDigits);
	FText IntToText(const int32& Value);
	FText IntToTextWithPlus(const int32& Value);

	UFUNCTION() void SelectUpgradeOne();
	UFUNCTION() void SelectUpgradeTwo();
	UFUNCTION() void SelectUpgradeThree();
	UFUNCTION() void SelectUpgradeFour();
	UFUNCTION() void SelectUpgradeFive();
};
