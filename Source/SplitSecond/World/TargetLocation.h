// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetLocation.generated.h"

DECLARE_DYNAMIC_DELEGATE(FTargetLocationDelegate);

UCLASS()
class SPLITSECOND_API ATargetLocation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATargetLocation();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USphereComponent* Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Root;

	FTargetLocationDelegate OnTargetLocationCollision;

private:
	UFUNCTION()void OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


};
