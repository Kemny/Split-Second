// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Super_AI_Character.generated.h"

class ASuper_Gun;

UCLASS()
class SPLITSECOND_API ASuper_AI_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASuper_AI_Character();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UHealthComponent* HealthComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UBoxComponent* TraceComp;

public:	
    /* Gun to give AI */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gun Settings")
    TSubclassOf<ASuper_Gun> AIGunClass;

    /* The Behavior Tree the AI will use */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
    class UBehaviorTree* BehaviorTreeToUse;

    /* the maximum amount of distance the AI can be from it's target */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
    float MaxTargetDistance;

    /* whether or not the AI is able to rotate to player */
    UPROPERTY(BlueprintReadWrite)
    bool bAbleToRotate;

    /* Enemies gun fire delay */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
    float GunFireDelay;

    /* AI's current gun */
    UPROPERTY(BlueprintReadWrite, Category = "Gun Vars")
    ASuper_Gun* CurrentGun;

    /* Fires the currently equipped gun */
    UFUNCTION(BlueprintCallable, Category = "Gun Functions")
    void FireGun();

public:

    UFUNCTION(BlueprintCallable, Category = "Health")
    FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

    void Highlight();
    void GetSlowed(float SlowTime, float SlowAmmount);
    UFUNCTION() void StopBeingSlowed();
    bool GetIsSlowed() const { return bIsSlowed; }

protected:

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  void Tick(float DeltaTime) override;

  UPROPERTY(BlueprintReadWrite)
  FRotator TargetRotation;
  UPROPERTY(EditAnywhere, Category = "AI Settings")
  float RotationSpeed = 2;

private:
    bool bIsSlowed;
    void SpawnGun();

};
