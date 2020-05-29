// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007


#include "Super_Boss.h"
#include "../SplitSecondGameMode.h"
#include "../Health/HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/PlayerCharacter.h"
#include "Engine/World.h"
#include "../SplitSecondPlayerState.h"
void ASuper_Boss::GetSlowed(float SlowTime, float SlowAmmount)
{
	SlowTime /= SlowDivider;
	Super::GetSlowed(SlowTime, SlowAmmount);
}

void ASuper_Boss::ScaleEnemyHealth(float BaseValue)
{
	if (!ensure(Gamemode != nullptr)) { return; }

	if (auto Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		if (auto PlayerPawn = Cast<APlayerCharacter>(Pawn))
		{
			auto PlayerStats = PlayerPawn->GetPlayerState<ASplitSecondPlayerState>()->CurrentStats;
			float NewValue = (PlayerStats.MaxHealth / 100 * BaseValue * Gamemode->BossHealthScaler) + BaseValue;
			HealthComponent->ChangeMaxHealth(NewValue);
		}
	}
}

void ASuper_Boss::ScaleEnemyDamage(float BaseValue)
{
	if (!ensure(Gamemode != nullptr)) { return; }

	if (auto Pawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		if (auto PlayerPawn = Cast<APlayerCharacter>(Pawn))
		{
			auto PlayerStats = PlayerPawn->GetPlayerState<ASplitSecondPlayerState>()->CurrentStats;
			float NewValue = (PlayerStats.Damage * PlayerStats.FireRate / 100 * BaseValue * Gamemode->BossDamageScaler) + BaseValue;
			Damage = NewValue;
		}
	}
	if (!ensure(Gamemode != nullptr)) { return; }
	float NewValue = (Gamemode->GetArenaNum() / 10 * BaseValue * Gamemode->BossDamageScaler) + BaseValue;
	
}
