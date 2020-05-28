// This project falls under CC-BY-SA lisence


#include "Projectile_Explosion.h"
#include "Kismet/GameplayStatics.h"
#include "../../../AI/Super_AI_Character.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "GameFramework/DamageType.h"

// Sets default values
AProjectile_Explosion::AProjectile_Explosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ExplosionRadiusCollision = CreateDefaultSubobject<USphereComponent>("Explosion Radius");
	RootComponent = ExplosionRadiusCollision;
	ExplosionRadiusCollision->SetSphereRadius(500);

	ExplosionRadiusCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ExplosionRadiusCollision->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AProjectile_Explosion::ApplyExplosionDamage(float Damage, float ExplosionUpTime, float Radius)
{
	SetLifeSpan(ExplosionUpTime);

	ExplosionRadiusCollision->SetSphereRadius(Radius);

	ExplosionRadiusCollision->GetOverlappingActors(ActorsToDamage, ASuper_AI_Character::StaticClass());

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	for (auto ActorToDamage : ActorsToDamage)
	{
		if (ActorToDamage)
		{
			UGameplayStatics::ApplyDamage(ActorToDamage, Damage, PlayerController, this, UDamageType::StaticClass());
		}
	}
}
