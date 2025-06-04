// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetHealthBarProgress(float Percent);
	void SetStaminaBarProgress(float Percent);

	void SetGoldText(int32 Gold);
	void SetSoulsText(int32 Souls);

private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaProgressBar;


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulsText;
};
