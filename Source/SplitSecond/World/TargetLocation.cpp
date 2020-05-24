// This project falls under CC-BY-SA lisence

#include "TargetLocation.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ATargetLocation::ATargetLocation()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Capsule"));
	Sphere->SetupAttachment(Root);
	Sphere->OnComponentBeginOverlap.AddUniqueDynamic(this, &ATargetLocation::OnCollision);
	Sphere->SetSphereRadius(200);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel4, ECollisionResponse::ECR_Ignore);

	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleSystem_Class(TEXT("/Game/Particles/P_Objective_Circle"));
	if (ParticleSystem_Class.Object) ParticleSystem = ParticleSystem_Class.Object;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System Component"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->SetTemplate(ParticleSystem);
}

void ATargetLocation::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		OnTargetLocationCollision.ExecuteIfBound();
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Destroy();
	}
}
