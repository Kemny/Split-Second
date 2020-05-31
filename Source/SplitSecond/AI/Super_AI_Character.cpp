// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007


#include "Super_AI_Character.h"
#include "../Weapons/Super_Gun.h"
#include "../Health/HealthComponent.h"
#include "Engine/World.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "SplitSecond_AI_Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "../AI/Super_Boss.h"
#include "Components/BoxComponent.h"
#include "TimerManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "../SplitSecondGameMode.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraComponent.h"

// Sets default values
ASuper_AI_Character::ASuper_AI_Character()
{

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = ASplitSecond_AI_Controller::StaticClass();
	AIGunClass = ASuper_Gun::StaticClass();

	TraceComp = CreateDefaultSubobject<UBoxComponent>(TEXT("TraceComp"));
	TraceComp->SetupAttachment(GetCapsuleComponent());
	TraceComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TraceComp->SetCollisionObjectType(ECC_WorldDynamic);
	TraceComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	TraceComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	TraceComp->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	TraceComp->SetBoxExtent(FVector(32, 32, 80));

    OnTakeAnyDamage.AddUniqueDynamic(this, &ASuper_AI_Character::OnTakeDamage);

	GunFireDelay = 2.0f;
	MaxTargetDistance = 2000.f;

	GetCharacterMovement()->bUseRVOAvoidance = true;
}

void ASuper_AI_Character::BeginPlay()
{
	Super::BeginPlay();

    switch (FMath::RandRange(0, 1))
    {
    case 0:
        SpawningMaterials.Add(GetMesh()->CreateAndSetMaterialInstanceDynamic(1));
        break;
    case 1:
        SpawningMaterials.Add(GetMesh()->CreateAndSetMaterialInstanceDynamic(0));
        SpawningMaterials.Add(GetMesh()->CreateAndSetMaterialInstanceDynamic(1));
        break;
    default:
        break;
    }

    bIsSpawning = true;

    if (!ensure(SpawnSound != nullptr)) { return; }
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpawnSound, GetActorLocation());

	SpawnGun();

    Gamemode = Cast<ASplitSecondGameMode>(UGameplayStatics::GetGameMode(this));
    if (!ensure(Gamemode != nullptr)) { return; }

    if (Gamemode->GetIsGameSlowed())
    {
        Gamemode->AddActorToSlowedArray(this);
    }

    ScaleEnemyDamage(Damage);

    ScaleEnemyHealth(MaxHealth);
}

void ASuper_AI_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsSpawning)
    {
        SpawnProgress = FMath::Clamp<float>(SpawnProgress - DeltaTime / SpawnTime, 0, 1);
        for (auto& SpawnMaterial : SpawningMaterials)
        {
            SpawnMaterial->SetScalarParameterValue(TEXT("Amount"), SpawnProgress);
        }
        if (SpawnProgress == 0)
        {
            if (!ensure(Body != nullptr)) { return; }
            if (!ensure(Wireframe != nullptr)) { return; }
            GetMesh()->SetMaterial(0, Body);
            GetMesh()->SetMaterial(1, Wireframe);

            SpawnDefaultController();

            bIsSpawning = false;
        }
    }

    RotateToPlayer(DeltaTime);
    CheckGameSlowTimers();
}

void ASuper_AI_Character::RotateToPlayer(float DeltaTime)
{
    if (bAbleToRotate && !bIsDead)
    {
        if (auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
        {
            auto Rotation = FMath::RInterpTo(GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()), DeltaTime, RotationSpeed);
            if (FMath::Abs(Rotation.Yaw - UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()).Yaw) <= MinRotationToFacePlayer)
            {
                bIsFacingPlayer = true;
            }
            else
            {
                bIsFacingPlayer = false;
            }

            SetActorRelativeRotation(FRotator(0, Rotation.Yaw, 0));
        }
    }
}

void ASuper_AI_Character::CheckGameSlowTimers()
{
    if (!ensure(Gamemode != nullptr)) { return; }

    if (bIsSlowed && GetWorld()->GetTimeSeconds() >= SlowTimerTargetTime)
    {
        bIsSlowed = false;
        Highlight(EHighlightType::NONE);

        if (Gamemode->GetIsGameSlowed())
        {
            Gamemode->AddActorToSlowedArray(this);
        }
        else
        {
            CustomTimeDilation = 1;
        }
    }
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
            auto SpawnTransform = FTransform(FRotator(0), FVector(0), FVector(1));

            CurrentGun = GetWorld()->SpawnActor<ASuper_Gun>(AIGunClass, SpawnTransform, ActorSpawnParams);

            CurrentGun->SetCurrentPawn(this);
            CurrentGun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), GunSocket);
        }
    }
}

void ASuper_AI_Character::Highlight(EHighlightType HighlightType)
{
    if (bIsSpawning) return;
    if (bIsSlowed) HighlightType = EHighlightType::Slow;

    float HealthPercentage = Health / MaxHealth;
    HealthPercentage = FMath::Clamp<float>(HealthPercentage, MinEmmision, 1);

    GetMesh()->CreateAndSetMaterialInstanceDynamic(1)->SetScalarParameterValue(TEXT("Emission Multiplier"), HealthPercentage);

    switch (HighlightType)
    {
    case EHighlightType::NONE:
        GetMesh()->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), DefaultColor * HealthPercentage);
        break;
    case EHighlightType::Highlight:
        GetMesh()->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), HighlightColor * HealthPercentage);
        break;
    case EHighlightType::Slow:
        GetMesh()->CreateAndSetMaterialInstanceDynamic(1)->SetVectorParameterValue(FName(TEXT("Color")), SlowedColor * HealthPercentage);
        break;
    default:
        break;
    }
}

void ASuper_AI_Character::GetSlowed(float SlowTime, float SlowAmmount)
{
    if (bIsSpawning) return;
    if (!GetMesh()) return;

    bIsSlowed = true;

    float HealthPercentage = Health / MaxHealth;
    Highlight(EHighlightType::Slow);

    CustomTimeDilation = SlowAmmount;
    SlowTimerTargetTime = GetWorld()->GetTimeSeconds() + SlowTime;
}

void ASuper_AI_Character::Destroyed()
{
    if (CurrentGun)
    {
        CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        CurrentGun->Destroy();
    }

    Super::Destroyed();
}

void ASuper_AI_Character::OnTakeDamage(AActor* DamagedActor, float NewDamage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (bIsSpawning || NewDamage <= 0) return;
    if (!ensure(DamageCauser != nullptr)) { return; }

    Health = FMath::Clamp(Health - NewDamage, 0.0f, MaxHealth);

    Highlight(EHighlightType::NONE);

    if (Health <= 0)
    {
        if (!ensure(Gamemode != nullptr)) { return; }
        if (!ensure(DeathParticles != nullptr)) { return; }
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), DeathParticles, GetActorLocation(), FRotator(0), FVector(1), true, true, ENCPoolMethod::None);

        Die();
    }
}

void ASuper_AI_Character::Die()
{
    if (!ensure(DeathSounds.Num() > 0)) { return; }
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSounds[FMath::RandRange(0, DeathSounds.Num() - 1)], GetActorLocation());

    bIsDead = true;
    SetActorEnableCollision(false);
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    DetachFromControllerPendingDestroy();
    
    Destroy();
}

void ASuper_AI_Character::ScaleEnemyHealth(float BaseValue)
{
    if (!ensure(Gamemode != nullptr)) { return; }
    float NewValue = (Gamemode->GetArenaNum() / 10 * BaseValue * Gamemode->EnemyHealthScaler) + BaseValue;
    Health += NewValue;
    MaxHealth = Health;
}

void ASuper_AI_Character::ScaleEnemyDamage(float BaseValue)
{
    if (!ensure(Gamemode != nullptr)) { return; }
    float NewValue = (Gamemode->GetArenaNum() / 10 * BaseValue * Gamemode->EnemyDamageScaler) + BaseValue;
    Damage = NewValue;
}
