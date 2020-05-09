// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SplitSecondProjectile.h"
#include "../Weapons/BulletMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "../Player/SplitSecondPlayerController.h" 
#include "GameFramework/DamageType.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "../Player/PlayerCharacter.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

ASplitSecondProjectile::ASplitSecondProjectile() 
{
	ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystem_BP(TEXT("/Game/Particles/NS_BulletHit.NS_BulletHit"));
	if (NiagaraSystem_BP.Object) NiagaraSystem = NiagaraSystem_BP.Object;

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

void ASplitSecondProjectile::GetSlowed(float SlowTime, float SlowAmmount)
{
	if (GetWorldTimerManager().IsTimerActive(SlowTimerHandle)) return;

	BulletMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0, 0, 1, 1));
	BulletMesh->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0, 0, 1, 1));

	CustomTimeDilation = SlowAmmount;
	GetWorldTimerManager().SetTimer(SlowTimerHandle, this, &ASplitSecondProjectile::StopBeingSlowed, SlowTime, false);
}
void ASplitSecondProjectile::StopBeingSlowed()
{ 
	CustomTimeDilation = 1;
	BulletMesh->CreateAndSetMaterialInstanceDynamic(0)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor::Red);
	BulletMesh->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor::Red);
}

void ASplitSecondProjectile::OnBulletHit_Implementation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
  APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
  if (!ensure(PlayerController != nullptr)) { return; }

  UGameplayStatics::ApplyDamage(OtherActor, DamageValue, PlayerController, this, UDamageType::StaticClass());
}
