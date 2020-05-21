// This project falls under CC-BY-SA lisence


#include "ExploadingProjectileBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Components/SphereComponent.h"

AExploadingProjectileBase::AExploadingProjectileBase()
{
	ExplosionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Explosion Radius"));
	ExplosionSphere->SetupAttachment(RootComponent);
}

void AExploadingProjectileBase::OnBulletHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) { return; }

	if (!ensure(ExplosionSphere != nullptr)) { return; }

	TArray<AActor*> IgnoredActors;

	UGameplayStatics::ApplyRadialDamage(this, DamageValue, GetActorLocation(), ExplosionSphere->GetScaledSphereRadius(), UDamageType::StaticClass(), IgnoredActors);
}
