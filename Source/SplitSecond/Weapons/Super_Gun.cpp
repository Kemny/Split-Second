// This project falls under CC-BY-SA lisence


#include "Super_Gun.h"
#include "Components/StaticMeshComponent.h"
#include "../Player/PlayerCharacter.h"
#include "../AI/Super_AI_Character.h"
#include "../Player/SplitSecondPlayerController.h"
#include "TimerManager.h"

// Sets default values
ASuper_Gun::ASuper_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
    RootComponent = GunMesh;

    FireRate = 0.5f;

    DefaultAmmoCount = 10;
    CurrentAmmoCount = DefaultAmmoCount;
    CurrentAmmoMax = DefaultAmmoCount;
}

void ASuper_Gun::OnInputPressed_Implementation()
{
  float FirstDelay = FMath::Max(LastTimeFired + FireRate - GetWorld()->TimeSeconds, 0.0f);

  GetWorldTimerManager().SetTimer(FireRateTimer, this, &ASuper_Gun::FireGun, FireRate, true, FirstDelay);
}

void ASuper_Gun::OnInputReleased_Implementation()
{
  GetWorldTimerManager().ClearTimer(FireRateTimer);
}

ASuper_Gun* ASuper_Gun::EquipGun(ACharacter* Character)
{
  if (!ensure(Character != nullptr)) { return nullptr; }

  APlayerCharacter* PlayerRef = Cast<APlayerCharacter>(Character);
  ASuper_AI_Character* AIRef = Cast<ASuper_AI_Character>(Character);

  if (PlayerRef)
  {
    PlayerRef->CurrentGun = this;
    CurrentPawn = PlayerRef;

    this->AttachToComponent(PlayerRef->GunAttachMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
    GunMesh->bCastDynamicShadow = false;
    GunMesh->CastShadow = false;
    
    return this;
  }
  else if (AIRef)
  {
    AIRef->CurrentGun = this;
    CurrentPawn = AIRef;

    USceneComponent* EnemyScene = Cast<USceneComponent>(AIRef->GetMesh());
    if (!ensure(EnemyScene != nullptr)) { return nullptr; }

    this->AttachToComponent(EnemyScene, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GunSocket"));
    GunMesh->bCastDynamicShadow = false;
    GunMesh->CastShadow = false;

    return this;
  }
  else
  {
    return nullptr;
  }
}

ACharacter* ASuper_Gun::GetCurrentPawn()
{
  return CurrentPawn;
}
