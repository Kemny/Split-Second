// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SplitSecondProjectile.h"
#include "../Weapons/BulletMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

ASplitSecondProjectile::ASplitSecondProjectile() 
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASplitSecondProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	BulletMovement = CreateDefaultSubobject<UBulletMovementComponent>(TEXT("BulletComp"));
	BulletMovement->UpdatedComponent = CollisionComp;
	BulletMovement->InitialSpeed = 3000.f;
	BulletMovement->MaxSpeed = 3000.f;
	BulletMovement->bRotationFollowsVelocity = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 0;

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	BulletMesh->SetupAttachment(CollisionComp);
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASplitSecondProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor) return;
	// Only add impulse and destroy projectile if we hit a physics
	if (OtherActor != this)
	{
		if (OtherActor->IsA<ASplitSecondProjectile>())
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleOnDeath, OtherActor->GetActorLocation());
			OtherActor->Destroy();
		}

		if (!ensure(ParticleOnDeath != nullptr)) { return; }
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleOnDeath, GetActorLocation());

		Destroy();
	}
}

void ASplitSecondProjectile::GetSlowed(float SlowTime, float SlowAmmount)
{
	bIsSlowed = true;
	BulletMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0, 0, 1, 1));
	BulletMesh->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0, 0, 1, 1));

	FTimerHandle TimerHandle;
	CustomTimeDilation = SlowAmmount;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASplitSecondProjectile::StopBeingSlowed, SlowTime, false);
}
void ASplitSecondProjectile::StopBeingSlowed()
{ 
	bIsSlowed = false;
	CustomTimeDilation = 1;
	BulletMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor::Red);
	BulletMesh->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor::Red);
}