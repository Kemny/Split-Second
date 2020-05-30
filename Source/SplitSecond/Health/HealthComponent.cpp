// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "HealthComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	AActor* Owner = GetOwner();
	if (Owner) 
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::TakeDamage);
	}
}

void UHealthComponent::TakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0)
	{
		return;
	}

	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);

	OnHealthChanged.ExecuteIfBound();
}

void UHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0)
	{
		return;
	}

	float NewHealth = Health + HealAmount;
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);

	OnHealthChanged.ExecuteIfBound();
}

