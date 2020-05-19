// This project falls under CC-BY-SA lisence

#include "FlagTarget.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

AFlagTarget::AFlagTarget()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(Root);
	Capsule->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFlagTarget::OnCollision);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);


	ConstructorHelpers::FObjectFinder<UStaticMesh> BP_Tablet(TEXT("/Game/Meshes/SM_Tablet"));
	if (BP_Tablet.Object) Mesh->SetStaticMesh(BP_Tablet.Object);
	ConstructorHelpers::FObjectFinder<UMaterialInterface> BP_Mat1(TEXT("/Game/Materials/MI_TransparentBorderBorder"));
	if (BP_Mat1.Object) Mesh->SetMaterial(0, BP_Mat1.Object);
	if (BP_Mat1.Object) Mesh->SetMaterial(1, BP_Mat1.Object);
	if (BP_Mat1.Object) Mesh->SetMaterial(2, BP_Mat1.Object);


	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));
}

void AFlagTarget::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		OnFlagTargetCollision.ExecuteIfBound();
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Destroy();
	}
}