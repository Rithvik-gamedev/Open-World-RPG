// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "Bird.generated.h"

class UInputMappingContext;
class UCapsuleComponent;
class USkeletalMeshcomponent;
class UCameraComponent;
class USpringArmComponent;
class UEnhancedInput;

UCLASS()
class RPG_API ABird : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABird();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void BirdMove(const FInputActionInstance& Value);
	void BirdMoveRight(const FInputActionInstance& Value);
	void BirdMouseLook(const FInputActionInstance& Value);

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* BirdMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* BirdMoveInput;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* BirdMouseInput;

private:

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* EagleMesh;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

};
