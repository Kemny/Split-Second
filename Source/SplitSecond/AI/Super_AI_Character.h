// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Super_AI_Character.generated.h"

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

    /* If true AI will randomize it's gun fire delay between a min and max value */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
    bool bUseRandomGunDelay = true;

	/* If UseRandomGunDelay is true this is the lowest value the gun fire delay can be  */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings", meta = (EditCondition = "bUseRandomGunDelay"))
    float MinGunFireDelay = 0.1;

    /* If UseRandomGunDelay is true this is the Highest value the gun fire delay can be  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings", meta = (EditCondition = "bUseRandomGunDelay"))
	float MaxGunFireDelay = 2;

    /* Enemies gun fire delay */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Settings", meta = (EditCondition = "!bUseRandomGunDelay"))
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

    UFUNCTION(BlueprintCallable, Category = "AI Functions")
    virtual void ScaleEnemyHealth(float BaseValue);

	UFUNCTION(BlueprintCallable, Category = "AI Functions")
	virtual void ScaleEnemyDamage(float BaseValue);

	/* Damage the AI applies to the player */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Vars")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "FX")
	class UNiagaraSystem* NiagaraSystem;

public:

    UFUNCTION(BlueprintCallable, Category = "Health")
    FORCEINLINE class UHealthComponent* GetHealthComponent() const { return HealthComponent; }

    void Highlight(EHighlightType HighlightType);
    virtual void GetSlowed(float SlowTime, float SlowAmmount);
    bool GetIsSlowed() const { return bIsSlowed; }
    
    UPROPERTY(BlueprintReadWrite)
    bool bIsDead = false;

    float SlowTimerTargetTime;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    void Tick(float DeltaTime) override;
    void Destroyed() override;

    UPROPERTY(EditAnywhere, Category = "AI Settings")
    float RotationSpeed = 2;
    UPROPERTY(EditAnywhere, Category = "AI Settings")
    float MinRotationToFacePlayer = 2;

    UPROPERTY(EditAnywhere, Category = "Colors")
    FLinearColor DefaultColor = FLinearColor::Red;
    UPROPERTY(EditAnywhere, Category = "Colors")
    FLinearColor HighlightColor = FLinearColor(0, 1, 1, 1);
    UPROPERTY(EditAnywhere, Category = "Colors")
    FLinearColor SlowedColor = FLinearColor::Blue;
    UPROPERTY(EditAnywhere, Category = "Colors")
    float MinEmmision = 0.3;
    UPROPERTY(EditAnywhere, Category = "Sound")
    TArray<USoundBase*> DeathSounds;
    UPROPERTY(EditAnywhere, Category = "Sound")
    USoundBase* SpawnSound;

	class ASplitSecondGameMode* Gamemode;

    void CheckGameSlowTimers();

private:
    bool bIsSlowed = false;
    bool bIsFacingPlayer = false;

    void SpawnGun();

    UFUNCTION() virtual void OnTakeDamage();
    void Die();

public:
    bool IsFacingPlayer() { return bIsFacingPlayer; }

};
