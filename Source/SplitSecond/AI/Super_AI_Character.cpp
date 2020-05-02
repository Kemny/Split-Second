// This project falls under CC-BY-SA lisence


#include "Super_AI_Character.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ASuper_AI_Character::ASuper_AI_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASuper_AI_Character::BeginPlay()
{
	Super::BeginPlay();

  
	
  UE_LOG(LogTemp, Log, TEXT("ID %i"), ID)
}

int32 ASuper_AI_Character::GetID()
{
  return ID;
}
