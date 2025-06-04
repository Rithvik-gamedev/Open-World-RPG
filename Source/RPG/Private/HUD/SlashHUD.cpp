// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashHUD.h"
#include "HUD/PlayerOverlay.h"

void ASlashHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController && SlashOverlayClass)
		{
			SlashOverlay = CreateWidget<UPlayerOverlay>(PlayerController, SlashOverlayClass);
			SlashOverlay->AddToViewport();

		}
	}

}
