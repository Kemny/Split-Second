// This project falls under CC-BY-SA lisence


#include "Super_Boss.h"
#include "../SplitSecondGameMode.h"
#include "../Health/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

void ASuper_Boss::ResetTimeDilation()
{
	UGameplayStatics::SetGlobalTimeDilation(this, 1);
}

void ASuper_Boss::ScaleEnemyHealth(float BaseValue)
{
	if (!ensure(Gamemode != nullptr)) { return; }
	float NewValue = (Gamemode->GetArenaNum() / 10 * BaseValue * Gamemode->BossHealthScaler) + BaseValue;
	HealthComponent->ChangeMaxHealth(NewValue);
}

void ASuper_Boss::ScaleEnemyDamage(float BaseValue)
{
	if (!ensure(Gamemode != nullptr)) { return; }
	float NewValue = (Gamemode->GetArenaNum() / 10 * BaseValue * Gamemode->BossDamageScaler) + BaseValue;
	Damage = NewValue;
}
