// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SlashAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/RPG_Character.h"

void USlashAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SlashCharacter = Cast<ARPG_Character>(TryGetPawnOwner());
	if (SlashCharacter)
	{
		SlashMovementComponent = SlashCharacter->GetCharacterMovement();
		
	}

}

void USlashAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if(SlashMovementComponent)
	{
		CharacterSpeed = UKismetMathLibrary::VSizeXY(SlashMovementComponent->Velocity);
		IsFalling = SlashMovementComponent->IsFalling();
		CharacterState = SlashCharacter->GetCharacterState();
		ActionState = SlashCharacter->GetActionState();
		DeathPose = SlashCharacter->GetDeathPose();
			
	}

}
