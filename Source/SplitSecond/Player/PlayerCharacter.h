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
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

  /* The player's current gun */
  UPROPERTY(BlueprintReadWrite, Category = "Gun Var's")
  ASuper_Gun* CurrentGun;

  /* This is the mesh the gun is attached to */
  UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
  class UStaticMeshComponent* GunAttachMesh;

protected:
	virtual void BeginPlay() override;

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

private:
	class ASplitSecondGameState* GameState;
};

