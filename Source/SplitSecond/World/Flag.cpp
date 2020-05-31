// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#include "Flag.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

AFlag::AFlag()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(Root);
	Capsule->OnComponentBeginOverlap.AddUniqueDynamic(this, &AFlag::OnCollision);
	Capsule->SetCapsuleRadius(80);
	Capsule->SetCapsuleHalfHeight(100);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Capsule);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> BP_Tablet(TEXT("/Game/Meshes/SM_Tablet"));
	if (BP_Tablet.Object) Mesh->SetStaticMesh(BP_Tablet.Object);
	ConstructorHelpers::FObjectFinder<UMaterialInterface> BP_Mat1(TEXT("/Game/Materials/M_MeshBody"));
	if (BP_Mat1.Object) Mesh->SetMaterial(0, BP_Mat1.Object);
	ConstructorHelpers::FObjectFinder<UMaterialInterface> BP_Mat2(TEXT("/Game/Materials/M_MeshWireframe"));
	if (BP_Mat2.Object) Mesh->SetMaterial(1, BP_Mat2.Object);
	ConstructorHelpers::FObjectFinder<UMaterialInterface> BP_Mat3(TEXT("/Game/Materials/Screen"));
	if (BP_Mat3.Object) Mesh->SetMaterial(2, BP_Mat3.Object);

	ConstructorHelpers::FObjectFinder<USoundBase> BP_CollisionSound(TEXT("/Game/Audio/2D/picktablet_Cue"));
	if (BP_CollisionSound.Object) CollisionSound = BP_CollisionSound.Object;

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotating Movement"));
}

void AFlag::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (!ensure(CollisionSound != nullptr)) { return; }
		UGameplayStatics::PlaySound2D(GetWorld(), CollisionSound);

		OnFlagCollision.ExecuteIfBound();
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Destroy();
	}
}
