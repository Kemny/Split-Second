// This project falls under CC-BY-SA lisence


#include "Super_AI_Character.h"
#include "../Weapons/Super_Gun.h"
#include "../Health/HealthComponent.h"
#include "Engine/World.h"
#include "SplitSecond_AI_Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"

#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASuper_AI_Character::ASuper_AI_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

  AIControllerClass = ASplitSecond_AI_Controller::StaticClass();
  AIGunClass = ASuper_Gun::StaticClass();

  HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));

  GunScale = FVector(1);

  GetCharacterMovement()->bUseRVOAvoidance = true;
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
  if (AIGunClass)
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
}

void ASuper_AI_Character::GetSlowed(float SlowTime, float SlowAmmount)
{
    bIsSlowed = true;
    GetMesh()->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor(0, 0, 1, 1));

    FTimerHandle TimerHandle;
    CustomTimeDilation = SlowAmmount;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &ASuper_AI_Character::StopBeingSlowed, SlowTime, false);
}
void ASuper_AI_Character::StopBeingSlowed() 
{ 
    bIsSlowed = false;

    GetMesh()->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), FLinearColor::Red);
    CustomTimeDilation = 1; 
}
