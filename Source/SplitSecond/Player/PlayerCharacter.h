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

public:
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

  

  /* This is the mesh the gun is attached to */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class UStaticMeshComponent* GunAttachMesh;

  /* Multiplier used to launch the player either right or left */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash Settings")
  float DashMultiplier;

  /* If dash button is hit twice within this delay it counts as a double tap  */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dash Settings")
  float DashInputDelay;

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun Settings")
    TSubclassOf<ASuper_Gun> GunClass;
    /* The player's current gun */
    UPROPERTY(BlueprintReadWrite, Category = "Gun Var's")
    ASuper_Gun* CurrentGun;

	/* If player has a gun this calls the gun's input pressed function */
	void OnFirePressed();
    /* If player has a gun this calls the gun's input released function */
    void OnFireReleased();
	/** Handles moving forward/backward */
	void MoveForward(float Val);
	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

private:
	class UPlayerMovementComponent* PlayerMovementComponent;

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FORCEINLINE class UPlayerMovementComponent* GetMyMovementComponent() const { return PlayerMovementComponent; }

private:

  void DashRightPressed();

  void DashRightReleased();

  void ResetRightDash();

  void DashLeftPressed();

  void ResetLeftDash();

  void DashLeftReleased();

  int32 RightKeyCount;

  int32 LeftKeyCount;

  FTimerHandle RightDashTimerHandle;

  FTimerHandle LeftDashTimerHandle;

  class ASplitSecondPlayerController* PlayerController;
};

