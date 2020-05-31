// GNU GENERAL PUBLIC LICENSE Version 3, 29 June 2007

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UMG.h"

#include "TransitionScreen.generated.h"

DECLARE_DYNAMIC_DELEGATE(FTransitionScreenDelegate);

UCLASS()
class SPLITSECOND_API UTransitionScreen : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetAnimation* ResultAnimation;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_ArenaSuccess;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_ArenaFail;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* txt_ArenaNum;

	UPROPERTY(BlueprintReadWrite)
	bool WinAnimation = false;

public:
	FTransitionScreenDelegate OnTransitionFinished;

	void PlayWinAnimation(int32 ArenaNum);
	void PlayFailAnimation(int32 ArenaNum);

	UFUNCTION() void HandleAnimationFinished();

};
