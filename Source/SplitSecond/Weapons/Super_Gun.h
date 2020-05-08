// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
  TSubclassOf<class ASplitSecondProjectile> ProjectileClass;

  /** Sound to play each time we fire */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
  class USoundBase* FireSound;

  /* Fire rate of player's gun not enemies */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
  float FireRate;

  /* Function called to fire the gun */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  void FireGun();

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
  ACharacter* GetCurrentPawn();

  /* Equip the gun to the given character returns the equipped gun */
  UFUNCTION(BlueprintCallable, Category = "Gun Functions")
  ASuper_Gun* EquipGun(ACharacter* Character);

private:

  ACharacter* CurrentPawn;

  bool bCanFire;

  FTimerHandle FireRateTimer;

  float LastTimeFired;
};
