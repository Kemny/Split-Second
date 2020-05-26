// This project falls under CC-BY-SA lisence


#include "Super_Gun.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/MeshComponent.h"
#include "../Player/PlayerCharacter.h"
#include "../AI/Super_AI_Character.h"
#include "../Player/SplitSecondPlayerController.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "SplitSecondProjectile.h"
#include "Guns/Player/PlayerProjectile.h"
#include "BulletMovementComponent.h"
#include "../SplitSecondPlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Guns/AI/AIProjectile.h"

// Sets default values
ASuper_Gun::ASuper_Gun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    auto Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(Root);

    GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
    GunMesh->SetupAttachment(GetRootComponent());

    GunMesh->bCastDynamicShadow = false;
    GunMesh->CastShadow = false;

    ConstructorHelpers::FObjectFinder<UStaticMesh> BP_KnifeMesh(TEXT("/Game/Meshes/SM_Knife"));
    if (BP_KnifeMesh.Object) KnifeMesh = BP_KnifeMesh.Object;
}

void ASuper_Gun::BeginPlay()
{
    Super::BeginPlay();

    auto PlayerController = GetWorld()->GetFirstPlayerController<ASplitSecondPlayerController>();
    if (!ensure(PlayerController != nullptr)) { return; }
    PlayerState = PlayerController->GetPlayerState<ASplitSecondPlayerState>();
    if (!ensure(PlayerState != nullptr)) { return; }
}

void ASuper_Gun::OnInputPressed_Implementation()
{
    if (!ensure(PlayerState != nullptr)) { return; }
    float FirstDelay = FMath::Max(LastTimeFired + (1/PlayerState->CurrentStats.FireRate) - GetWorld()->TimeSeconds, 0.0f);

    GetWorldTimerManager().SetTimer(FireRateTimer, this, &ASuper_Gun::FireGun, 1/PlayerState->CurrentStats.FireRate, true, FirstDelay);
}

void ASuper_Gun::OnInputReleased_Implementation()
{
  GetWorldTimerManager().ClearTimer(FireRateTimer);
}

ACharacter* ASuper_Gun::GetCurrentPawn() const
{
  return CurrentPawn;
}

AAIProjectile* ASuper_Gun::AI_SpawnProjectile(FVector Offset)
{
    AAIProjectile* Projectile = nullptr;

    if (ProjectileClass != NULL)
    {
        UWorld* const World = GetWorld();
        if (World != NULL)
        {
            const FVector SpawnLocation = GunMesh->GetSocketLocation(FName("MuzzleLocation"));

            auto SpawnTransform = FTransform(UKismetMathLibrary::FindLookAtRotation(SpawnLocation, UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation() + Offset), SpawnLocation);

            Projectile = World->SpawnActorDeferred<AAIProjectile>(ProjectileClass, SpawnTransform);
            if (Projectile)
            {
                Projectile->SetCurrentAI(GetCurrentPawn());
                Projectile->ConstructEnemyProjectile();

                if (!ensure(ShootSound != nullptr)) { return nullptr; }
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootSound, Projectile->GetActorLocation());

                Projectile->FinishSpawning(SpawnTransform);
            }
        }
    }
    return Projectile;
}

APlayerProjectile* ASuper_Gun::Player_SpawnProjectile(UClass* Class, FVector const& Location, FRotator const& Rotation)
{
    FTransform SpawnTransform = FTransform(Rotation, Location, FVector(1));
    auto CurrentProjectile = GetWorld()->SpawnActorDeferred<APlayerProjectile>(ProjectileClass, SpawnTransform);

    if (CurrentProjectile)
    {
        if (auto PlayerCharacter = Cast<APlayerCharacter>(GetCurrentPawn()))
        {
            auto Upgrades = PlayerCharacter->GetPlayerState<ASplitSecondPlayerState>()->CurrentStats;

            CurrentProjectile->DamageValue = Upgrades.Damage;
            CurrentProjectile->GetProjectileMovement()->Velocity = CurrentProjectile->GetProjectileMovement()->Velocity.GetSafeNormal()* Upgrades.ProjectileSpeed;

            if (Upgrades.bIsPiercing)
            {
                CurrentProjectile->GetCollisionComp()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
                CurrentProjectile->GetBulletMesh()->SetStaticMesh(KnifeMesh);
                CurrentProjectile->GetBulletMesh()->SetWorldScale3D(FVector(2));

                CurrentProjectile->bIsPiercing = true;
            }

            CurrentProjectile->bIsExplosive = Upgrades.bExplodingBullets;

            if (Upgrades.bIsBouncing)
            {
                CurrentProjectile->bShouldBounce = true;
            }
            if (Upgrades.bIsHoming)
            {
                CurrentProjectile->GetProjectileMovement()->bIsHomingProjectile = true;
            }
        }
        if (!ensure(ShootSound != nullptr)) { return nullptr; }
        UGameplayStatics::PlaySound2D(GetWorld(), ShootSound);

        CurrentProjectile->FinishSpawning(SpawnTransform);
    }

    return CurrentProjectile;
}

void ASuper_Gun::AfterPlayerFireGun(UMeshComponent* GunMeshToEdit)
{
    if (!ensure(GunMeshToEdit != nullptr)) { return; }
    if (!ensure(PlayerState != nullptr)) { return; }

    PlayerState->CurrentStats.Ammo--;

    float AmmoPercentage = (float)PlayerState->CurrentStats.Ammo / (float)PlayerState->CurrentStats.MaxAmmo;
    GunMeshToEdit->CreateAndSetMaterialInstanceDynamic(1)->SetScalarParameterValue(TEXT("Emission Multiplier"), AmmoPercentage);
    GunMeshToEdit->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(TEXT("Color"), FLinearColor(AmmoPercentage, 0, 0, 1));
    LocalGunMeshToEdit = GunMeshToEdit;

    LastTimeFired = GetWorld()->TimeSeconds;

    // if gun is current regenerating ammo stop regeneration
    if (bReloadActive)
    {
        GetWorldTimerManager().ClearTimer(ReloadTimer);
        bReloadActive = false;
    }

    StartRegen();
}

void ASuper_Gun::StartRegen()
{
    ReloadSpeed = PlayerState->CurrentStats.ReloadSpeed;

    GetWorldTimerManager().SetTimer(ReloadTimer, this, &ASuper_Gun::RegenAmmo, 1/ReloadSpeed, true);
}

void ASuper_Gun::RegenAmmo()
{
    if (!ensure(LocalGunMeshToEdit != nullptr)) { return; }

    PlayerState->CurrentStats.Ammo = FMath::Clamp(PlayerState->CurrentStats.Ammo + 1, 0, PlayerState->CurrentStats.MaxAmmo);
    bReloadActive = true;

    float AmmoPercentage = (float)PlayerState->CurrentStats.Ammo / (float)PlayerState->CurrentStats.MaxAmmo;
    LocalGunMeshToEdit->CreateAndSetMaterialInstanceDynamic(1)->SetScalarParameterValue(TEXT("Emission Multiplier"), AmmoPercentage);
    LocalGunMeshToEdit->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(TEXT("Color"), FLinearColor(AmmoPercentage, 0, 0, 1));

    if (PlayerState->CurrentStats.Ammo >= PlayerState->CurrentStats.MaxAmmo)
    {
      GetWorldTimerManager().ClearTimer(ReloadTimer);

      bReloadActive = false;
    }
}

bool ASuper_Gun::IsOutOfAmmo()
{
    return PlayerState->CurrentStats.Ammo <= 0;
}
