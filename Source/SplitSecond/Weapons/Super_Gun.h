// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplitSecondProjectile.h" ///Compilation fail when forward declaring for some reason
#include "Super_Gun.generated.h"

class ACharacter;

UCLASS()
class SPLITSECOND_API ASuper_Gun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuper_Gun();

  UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
  class UStaticMeshComponent* GunMesh;

  /** Projectile class to spawn */
  UPROPERTY(EditDefaultsOnly, Category = "Projectile")
  TSubclassOf<ASplitSecondProjectile> ProjectileClass;

  /** Sound to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
  class USoundBase* FireSound;

  /* Fire rate of player's gun not enemies */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
  float FireRate;

  /* The default ammo count of this gun */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
  float DefaultAmmoCount;

  /* The current ammo count of this gun */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
  float CurrentAmmoCount;
  /* Current Maximum Ammo*/
  UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Projectile")
  float CurrentAmmoMax;

  /* Function called to fire the gun */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  virtual void FireGun() PURE_VIRTUAL(&ASuper_Gun::FireGun, );

  /* Called when player fire key is pressed */
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gun Events")
  void OnInputPressed();
  virtual void OnInputPressed_Implementation();

  /* Called when player fire key is released */
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Gun Events")
  void OnInputReleased();
  virtual void OnInputReleased_Implementation();

  /* Returns the pawn that currently has the gun */
  UFUNCTION(BlueprintPure, Category = "Gun Functions")
  ACharacter* GetCurrentPawn() const ;
  void SetCurrentPawn(ACharacter* NewPawn) { CurrentPawn = NewPawn; }

protected:
  ACharacter* CurrentPawn;

  bool bCanFire;

  FTimerHandle FireRateTimer;

  float LastTimeFired;

};
