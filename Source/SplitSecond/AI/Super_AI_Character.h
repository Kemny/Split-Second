// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Super_AI_Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAIDeathDelegate, ASuper_AI_Character*, KilledAI);

UENUM()
enum class EHighlightType
{
    NONE,
    Highlight,
    Slow
};

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

    /* Socket to to attach gun to */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings")
    FName GunSocket = "GunSocket";

    /* AI's current gun */
    UPROPERTY(BlueprintReadWrite, Category = "Gun Vars")
    ASuper_Gun* CurrentGun;

    /* Fires the currently equipped gun */
    UFUNCTION(BlueprintCallable, Category = "Gun Functions")
    void FireGun();

	/* Damage the AI applies to the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Vars")
	float Damage;

public:

    UFUNCTION(BlueprintCallable, Category = "Health")
    FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

    void Highlight(EHighlightType HighlightType);
    void GetSlowed(float SlowTime, float SlowAmmount);
    UFUNCTION() void StopBeingSlowed();
    bool GetIsSlowed() const { return bIsSlowed; }
    
    UPROPERTY(BlueprintReadWrite)
    bool bIsDead = false;

    FAIDeathDelegate OnDeath;
    FAIDeathDelegate OnDestroyed;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    void Tick(float DeltaTime) override;
    void Destroyed() override;

    UPROPERTY(EditAnywhere, Category = "AI Settings")
    float RotationSpeed = 2;

    UPROPERTY(EditAnywhere, Category = "Colors")
    FLinearColor DefaultColor = FLinearColor::Red;
    UPROPERTY(EditAnywhere, Category = "Colors")
    FLinearColor HighlightColor = FLinearColor(0, 1, 1, 1);
    UPROPERTY(EditAnywhere, Category = "Colors")
    FLinearColor SlowedColor = FLinearColor::Blue;
    UPROPERTY(EditAnywhere, Category = "Colors")
    float MinEmmision = 0.3;

    UPROPERTY(EditAnywhere, Category = "Health")
    float DeathDespawnTime = 5;

private:
    bool bIsSlowed = false;

    void SpawnGun();
    FRotator GetNewRot(FVector TargetPosition, FVector WorldUp = FVector::UpVector);

    UFUNCTION() virtual void OnTakeDamage();
    UFUNCTION() void DestroyAfterDeath();

};
