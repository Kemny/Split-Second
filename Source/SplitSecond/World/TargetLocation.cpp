// This project falls under CC-BY-SA lisence

#include "TargetLocation.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"

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
	Sphere->SetSphereRadius(1000);

	//TODO Add Particles

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
