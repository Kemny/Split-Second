// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

///Cannot be forward declared
#include "SplitSecondProjectile.h"
#include "Engine/World.h"

#include "Super_Gun.generated.h"

class ACharacter;

UCLASS()
class SPLITSECOND_API ASuper_Gun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASuper_Gun();

    void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* GunMesh;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<ASplitSecondProjectile> ProjectileClass;

	/* Function called to fire the gun */
	UFUNCTION(BlueprintCallable, Category = "Gun Functions")
	virtual void FireGun();

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

	virtual class UStaticMeshComponent* GetGunMesh() const { return GunMesh; }

protected:
    ACharacter* CurrentPawn;
    class ASplitSecondPlayerState* PlayerState;

	bool bCanFire;

	FTimerHandle FireRateTimer;

	float LastTimeFired;

	UStaticMesh* KnifeMesh;

	FRotator FindPlayerBulletRotation(class UMeshComponent* GunMeshToEdit, FVector Offset = FVector::ZeroVector);
	class APlayerProjectile* Player_SpawnProjectile(UClass* Class, FVector const& Location, FRotator const& Rotation);
	void AfterPlayerFireGun();

	class AAIProjectile* AI_SpawnProjectile(FVector Offset = FVector::ZeroVector);

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* ShootSound;
};

