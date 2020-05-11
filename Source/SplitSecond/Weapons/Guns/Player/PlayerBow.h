// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "../../Super_Gun.h"
#include "PlayerBow.generated.h"

/**
 * A Bow
 */
UCLASS()
class SPLITSECOND_API APlayerBow : public ASuper_Gun
{
	GENERATED_BODY()

public:
	APlayerBow();
	void Tick(float DeltaTime) override;

	void FireGun() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    class USkeletalMeshComponent* BowMesh;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHeld = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsDrawingBow = false;
	UPROPERTY(BlueprintReadWrite)
	float BowDrawPrecentage = 0;

	void OnInputPressed_Implementation() override;
	void OnInputReleased_Implementation() override;
};
