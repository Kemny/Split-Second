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
	Sphere->OnComponentHit.AddUniqueDynamic(this, &ATargetLocation::OnCollision);

}

void ATargetLocation::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		OnTargetLocationCollision.ExecuteIfBound();
		Destroy();
	}
}
