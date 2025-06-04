// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashHUD.generated.h"

class UPlayerOverlay;

UCLASS()
class RPG_API ASlashHUD : public AHUD
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = "Slash")
	TSubclassOf<UPlayerOverlay> SlashOverlayClass;

	UPROPERTY()
	UPlayerOverlay* SlashOverlay;

public:

	FORCEINLINE UPlayerOverlay* GetSlashPlayerOverlay() const { return SlashOverlay; }
	
};
