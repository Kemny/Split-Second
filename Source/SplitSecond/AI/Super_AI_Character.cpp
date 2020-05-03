// This project falls under CC-BY-SA lisence


#include "Super_AI_Character.h"
#include "../Weapons/Super_Gun.h"
#include "Engine/World.h"
#include "SplitSecond_AI_Controller.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ASuper_AI_Character::ASuper_AI_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  AIControllerClass = ASplitSecond_AI_Controller::StaticClass();
  AIGunClass = ASuper_Gun::StaticClass();

  GunScale = FVector(1);
  MaxHP = 10.f;
  CurrentHP = 10.f;
}

void ASuper_AI_Character::BeginPlay()
{
  Super::BeginPlay();

  SpawnGun();
}

void ASuper_AI_Character::FireGun()
{
  if (!ensure(CurrentGun != nullptr)) { return; }

  CurrentGun->FireGun();
}

void ASuper_AI_Character::SpawnGun()
{
  UWorld* const World = GetWorld();

  if (World != NULL)
  {
    //Set Spawn Collision Handling Override
    FActorSpawnParameters ActorSpawnParams;
    ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    auto SpawnTransform = FTransform(FRotator(0), FVector(0), GunScale);

    CurrentGun = GetWorld()->SpawnActor<ASuper_Gun>(AIGunClass, SpawnTransform, ActorSpawnParams);

    CurrentGun->EquipGun(this);
  }
}
