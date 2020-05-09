// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI_TraceComponent.generated.h"

/* This components can spawn a line trace and checks to see if player was hit */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPLITSECOND_API UAI_TraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAI_TraceComponent();

  /* The trace multiplier for line tracing */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret Settings")
  float TraceMultipler;

  /* Whether or not to draw debug lines will also log what the trace has hit */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret Settings")
  bool bDrawDebug;

  /* Function called to start line trace */
  UFUNCTION(BlueprintCallable, Category = "Trace Functions")
  void Trace(FVector StartLocation);

  /* Checks to see if the player was hit by the trace */
  UFUNCTION(BlueprintPure, Category = "Trace Functions")
  bool DidHitPlayer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
  
  bool bPlayerWasHit;

  AActor* Owner;

  FVector OwnerLocation;
};
