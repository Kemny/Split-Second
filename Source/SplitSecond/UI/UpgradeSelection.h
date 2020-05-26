// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UMG.h"
#include "../Libraries/SplitSecondWeapons.h"

#include "UpgradeSelection.generated.h"

DECLARE_DYNAMIC_DELEGATE(FUpgradeSelectionDelegate);

UENUM(BlueprintType)
enum class EArenaUpgrades : uint8
{
	NONE,
	Ammo,
	Damage,
	ProjectileSpeed,
	ReloadSpeed,
	FireRate,
	MaxJumps,
	BulletNum,
	BulletSpread,
	BowDrawSpeed,
};
UENUM(BlueprintType)
enum class EBosssUpgrades : uint8
{
	NONE,
	IsPiercing,
	IsHoming,
	IsBouncing,
	ExplodingBullets,
	ExtraLife,
	CanThrowGun,
};

USTRUCT(Blueprintable)
struct FUpgradeValue
{
	GENERATED_USTRUCT_BODY()

public:
	
	UPROPERTY(EditAnywhere)float MinValue = 0;
	UPROPERTY(EditAnywhere)float MaxValue = 1;
	UPROPERTY(EditAnywhere)float MaxDigits = 1;
};

UCLASS()
class SPLITSECOND_API UUpgradeSelection : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FUpgradeSelectionDelegate OnUpgradeSelected;

	void ShowUpgradeSelection(FUpgrades* CurrentUpgrades, const EWeapons& CurrentWeapon, const bool& bBossUpgrades, class UHealthComponent* HealthComponent);

protected:
	UPROPERTY(EditAnywhere)
	TMap<EArenaUpgrades, FUpgradeValue> UpgradeValues;

	UPROPERTY(EditAnywhere)
	int32 MAX_ATTEMPTS = 500;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* HealSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* SelectUpgradeSound;

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
	FUpgrades* PlayerUpgrades = nullptr;
	class UHealthComponent* PlayerHealthComponent = nullptr;
	EWeapons PlayerCurrentWeapon;
	TMap<EArenaUpgrades, float> ArenaUpgradeValues;
	TMap<EBosssUpgrades, bool> BossUpgradeValues;
	float UpgradeValue;

	EArenaUpgrades OneUpgrade;
	EArenaUpgrades TwoUpgrade;
	EArenaUpgrades ThreeUpgrade;
	EArenaUpgrades FourUpgrade;
	EArenaUpgrades FiveUpgrade;

	EBosssUpgrades OneUpgradeBoss;
	EBosssUpgrades TwoUpgradeBoss;
	EBosssUpgrades ThreeUpgradeBoss;

	FText BoolToText(const bool& BoolToConvert);
	FText FloatToText(const float& Value, const int32& FractalDigits);
	FString FloatToString(const float& Value, const int32& FractalDigits);
	FText IntToText(const int32& Value);
	FText IntToTextWithPlus(const int32& Value);

	void TryToCreateRandomUpgrade(int32 index);
	void TryToCreateBossUpgrade(int32 index);

	bool CreateRandomUpgrade(int32 index, EArenaUpgrades& OutType);
	bool CreateRandomBossUpgrade(int32 index, EBosssUpgrades& OutType);

	UFUNCTION() void SelectBossUpgradeOne();
	UFUNCTION() void SelectBossUpgradeTwo();
	UFUNCTION() void SelectBossUpgradeThree();
	void ApplyBossUpgrade(EBosssUpgrades TypeToApply);

	UFUNCTION() void SelectArenaUpgradeOne();
	UFUNCTION() void SelectArenaUpgradeTwo();
	UFUNCTION() void SelectArenaUpgradeThree();
	UFUNCTION() void SelectArenaUpgradeFour();
	UFUNCTION() void SelectArenaUpgradeFive();
	void ApplyArenaUpgrade(EArenaUpgrades TypeToApply);

	void SetBossTextUpgradeValue(UTextBlock* TextToSet, EBosssUpgrades TypeToApply);
	void SetArenaTextUpgradeValue(UTextBlock* TextToSet, EArenaUpgrades TypeToApply);

};
