// This project falls under CC-BY-SA lisence


#include "AI_TraceComponent.h"
#include "../SplitSecond.h"
#include "CollisionQueryParams.h"
#include "GameFramework/Actor.h"
#include "../Player/PlayerCharacter.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UAI_TraceComponent::UAI_TraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

  TraceMultipler = 500;
}

// Called when the game starts
void UAI_TraceComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
  if (!ensure(Owner != nullptr)) { return; }

  OwnerLocation = Owner->GetActorLocation();
}

void UAI_TraceComponent::Trace(FVector StartLocation)
{
  FHitResult OutHit;

  FVector ForwardVector = Owner->GetActorForwardVector();
  FVector End = ((ForwardVector * TraceMultipler) + StartLocation);
  FCollisionObjectQueryParams ObjectCollisionParams;
  ObjectCollisionParams.AddObjectTypesToQuery(ECC_Pawn);
  FCollisionQueryParams CollisionParams;
  CollisionParams.AddIgnoredActor(Owner);
  
  if (bDrawDebug)
  {
    DrawDebugLine(GetWorld(), StartLocation, End, FColor::Green, false, 1, 0, 1);
  }

  if (GetWorld()->LineTraceSingleByObjectType(OutHit, StartLocation, End, ObjectCollisionParams, CollisionParams))
  {
    if (OutHit.Actor != NULL)
    {
      if (OutHit.Actor->IsA<APlayerCharacter>())
      {
        bPlayerWasHit = true;
        
        if (bDrawDebug)
        {
          UE_LOG(LogAISystem, Log, TEXT("Hit player"))
        }
      }
      else
      {
        bPlayerWasHit = false;
        
        if (bDrawDebug)
        {
          UE_LOG(LogAISystem, Log, TEXT("Hit : %s"), *OutHit.Actor->GetName())
        }
      }
    }
    else
    {
      bPlayerWasHit = false;
      
      if (bDrawDebug)
      {
        UE_LOG(LogAISystem, Log, TEXT("No Hit"))
      }
    }
  }
}

bool UAI_TraceComponent::DidHitPlayer()
{
  return bPlayerWasHit;
}

