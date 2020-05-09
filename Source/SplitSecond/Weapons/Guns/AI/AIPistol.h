// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "../../Super_Gun.h"
#include "AIPistol.generated.h"

/**
 * 
 */
UCLASS()
class SPLITSECOND_API AAIPistol : public ASuper_Gun
{
	GENERATED_BODY()
	
public:
	void FireGun() override;
};
