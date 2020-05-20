// This project falls under CC-BY-SA lisence

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AI_Timeskip.generated.h"

UCLASS()
class SPLITSECOND_API AAI_Timeskip : public AActor
{
	GENERATED_BODY()
	
public:	

	UFUNCTION(BlueprintCallable, Category = "Time Functions")
	void SkipTime(float SkipAmount);

};
