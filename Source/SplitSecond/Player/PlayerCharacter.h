// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInputComponent;
class ASuper_Gun;

UCLASS(config=Game)
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    class USceneComponent* BossLookAtRight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* BossLookAtLeft;

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);


    /* This is the mesh the gun is attached to */
    UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
    class UStaticMeshComponent* GunAttachMesh;

    /* Multiplier used to launch the player either right or left */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
    float DashMultiplier;

    /* If dash button is hit twice within this delay it counts as a double tap  */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash Settings")
    float DashInputDelay;

    void EquipGun(TSubclassOf<class ASuper_Gun> GunClass);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

    void Jump() override;
    void Landed(const FHitResult& Hit) override;

    /* The player's current gun */
    UPROPERTY(BlueprintReadWrite, Category = "Gun Var's")
    ASuper_Gun* CurrentGun;
    
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* ExtraLifeSound;
	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* PlayerHitSound;

	/* If player has a gun this calls the gun's input pressed function */
	void OnFirePressed();
    /* If player has a gun this calls the gun's input released function */
    void OnFireReleased();
	/** Handles moving forward/backward */
	void MoveForward(float Val);
	/** Handles stafing movement, left and right */
	void MoveRight(float Val);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "Health")
    bool bIsInvinclible = false;

private:
	class UPlayerMovementComponent* PlayerMovementComponent;
    class UPlayerUI* PlayerUI;
    class UPauseMenu* PauseMenu;

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE class UPlayerMovementComponent* GetMyMovementComponent() const { return PlayerMovementComponent; }
	FORCEINLINE class UPlayerUI* GetPlayerUI() const { return PlayerUI; }
    UFUNCTION(BlueprintCallable, Category = "Weapons")
    FORCEINLINE ASuper_Gun* GetCurrentGun() const { return CurrentGun; }
    void SpawnPlayerUI(TSubclassOf<class UPlayerUI> UIToSpawn);

private:
    void ResetDash();

    int32 RightKeyCount;

    int32 LeftKeyCount;

    FTimerHandle DashTimerHandle;

    class ASplitSecondPlayerController* PlayerController;
    class ASplitSecondPlayerState* PlayerState;

    int32 CurrentJump;

    bool bCanDash = false;
    bool bDashOffCooldown = true;
    void AllowDash() { bCanDash = true; }
    void ForbidDash() { bCanDash = false; }

    UFUNCTION() void OnTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
    UFUNCTION() void OnConfirmedDeath();
};

