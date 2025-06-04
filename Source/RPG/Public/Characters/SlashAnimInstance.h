// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CharacterTypes.h"
#include "SlashAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class RPG_API USlashAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	class ARPG_Character* SlashCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	class UCharacterMovementComponent* SlashMovementComponent;


	UPROPERTY(BlueprintReadOnly, Category = "Character")
	float CharacterSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Character")
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly,Category = "Movement")
	ECharacterState CharacterState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EActionState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	TEnumAsByte<EDeathPose> DeathPose;
};
