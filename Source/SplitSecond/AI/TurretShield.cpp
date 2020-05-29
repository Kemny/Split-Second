// This project falls under CC-BY-SA lisence


#include "TurretShield.h"

// Sets default values
ATurretShield::ATurretShield()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATurretShield::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurretShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

