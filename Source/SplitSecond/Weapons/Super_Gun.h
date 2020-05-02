// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Super_Gun.generated.h"

UCLASS()
class SPLITSECOND_API ASuper_Gun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuper_Gun();

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  class USkeletalMeshComponent* GunMesh;

  /** Projectile class to spawn */
  UPROPERTY(EditDefaultsOnly, Category = "Projectile")
  TSubclassOf<class ASplitSecondProjectile> ProjectileClass;

  /** Sound to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
  class USoundBase* FireSound;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
  float FireRate;

  /* Function called to fire the gun */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  void FireGun();

};
