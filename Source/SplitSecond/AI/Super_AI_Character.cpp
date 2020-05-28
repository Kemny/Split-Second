// This project falls under CC-BY-SA lisence


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

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
    HealthComponent->OnHealthChanged.BindUFunction(this, TEXT("OnTakeDamage"));

	GunFireDelay = 2.0f;
	MaxTargetDistance = 2000.f;

	GetCharacterMovement()->bUseRVOAvoidance = true;
}

void ASuper_AI_Character::BeginPlay()
{
	Super::BeginPlay();

	SpawnGun();

    Gamemode = Cast<ASplitSecondGameMode>(UGameplayStatics::GetGameMode(this));

    ScaleEnemyDamage(Damage);

    if (!ensure(HealthComponent != nullptr)) { return; }
    ScaleEnemyHealth(HealthComponent->GetMaxHealth());
}

void ASuper_AI_Character::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bAbleToRotate && !bIsDead)
    {
        if (auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
        {
            auto Rotation = FMath::RInterpTo(GetActorRotation(), UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()), DeltaTime, RotationSpeed);
            if (FMath::Abs(Rotation.Yaw - UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()).Yaw)  <= MinRotationToFacePlayer)
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

    CheckGameSlowTimers();
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
            CustomTimeDilation = Gamemode->GetCurrentSlowValue();

            SlowTimerTargetTime = GetWorld()->GetTimeSeconds() + FMath::Abs(Gamemode->GetSlowEndTime() - GetWorld()->GetTimeSeconds());
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
    if (bIsSlowed) HighlightType = EHighlightType::Slow;

    float HealthPercentage = HealthComponent->GetHealth() / HealthComponent->GetMaxHealth();
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
    if (!GetMesh()) return;
    bIsSlowed = true;

    float HealthPercentage = HealthComponent->GetHealth() / HealthComponent->GetMaxHealth();
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

void ASuper_AI_Character::OnTakeDamage()
{
    Highlight(EHighlightType::NONE);

    if (HealthComponent->GetHealth() <= 0)
    {
        Die();
    }
}

void ASuper_AI_Character::Die()
{
    bIsDead = true;
    SetActorEnableCollision(false);
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    DetachFromControllerPendingDestroy();

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraSystem, GetActorLocation(), GetActorRotation(), FVector(1), true, true, ENCPoolMethod::AutoRelease);

    Destroy();
}

void ASuper_AI_Character::ScaleEnemyHealth(float BaseValue)
{
    if (!ensure(Gamemode != nullptr)) { return; }
    float NewValue = (Gamemode->GetArenaNum() / 10 * BaseValue * Gamemode->EnemyHealthScaler) + BaseValue;
    HealthComponent->ChangeMaxHealth(NewValue);
}

void ASuper_AI_Character::ScaleEnemyDamage(float BaseValue)
{
    if (!ensure(Gamemode != nullptr)) { return; }
    float NewValue = (Gamemode->GetArenaNum() / 10 * BaseValue * Gamemode->EnemyDamageScaler) + BaseValue;
    Damage = NewValue;
}
