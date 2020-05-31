// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SplitSecondProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/DamageType.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

ASplitSecondProjectile::ASplitSecondProjectile() 
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASplitSecondProjectile::OnBulletOverlap);
	CollisionComp->OnComponentHit.AddDynamic(this, &ASplitSecondProjectile::OnHit);
	
	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bTickBeforeOwner = false;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0;

	// Infinite lifespan
	InitialLifeSpan = 0;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BulletMesh->SetupAttachment(CollisionComp);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASplitSecondProjectile::OnBulletOverlap(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;
	if (OtherActor->IsA<ASplitSecondProjectile>()) return;

	UGameplayStatics::ApplyDamage(OtherActor, Damage, UGameplayStatics::GetPlayerController(GetWorld(), 0), this, UDamageType::StaticClass());

	if (UGameplayStatics::GetPlayerPawn(GetWorld(), 0) != OtherActor)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DefaultCollisionParticle, GetActorLocation(), GetActorRotation(), FVector(1), true, true, ENCPoolMethod::AutoRelease);
	}

	if (!ensure(BulletImpactSound != nullptr)) { return; }
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), BulletImpactSound, GetActorLocation());

	Destroy();
}
void ASplitSecondProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor) return;
	if (OtherActor->IsA<ASplitSecondProjectile>()) return;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DefaultCollisionParticle, GetActorLocation(), GetActorRotation(), FVector(1), true, true, ENCPoolMethod::AutoRelease);

	if (!ensure(BulletImpactSound != nullptr)) { return; }
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), BulletImpactSound, GetActorLocation());

	Destroy();
}