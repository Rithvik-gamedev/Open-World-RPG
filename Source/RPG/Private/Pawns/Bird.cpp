// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
 

// Sets default values
ABird::ABird()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComp);

	EagleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMesh"));
	EagleMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = true;

}

// Called when the game starts or when spawned
void ABird::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (SubSystem)
		{
			SubSystem->AddMappingContext(BirdMappingContext, 0);
		}

	}
	
}

void ABird::BirdMove(const FInputActionInstance& Value)
{
	const FVector2D ForwardVectorValue = Value.GetValue().Get<FVector2D>();

	if (GetController() != NULL)
	{
		AddMovementInput(GetActorForwardVector(), ForwardVectorValue.Y);
	}
}

void ABird::BirdMoveRight(const FInputActionInstance& Value)
{
	const FVector2D RightVectorValue = Value.GetValue().Get<FVector2D>();

	if (GetController() != NULL)
	{
		AddMovementInput(GetActorRightVector(), RightVectorValue.X);
	}

}

void ABird::BirdMouseLook(const FInputActionInstance& Value)
{
	const FVector2D MouseLook = Value.GetValue().Get<FVector2D>();

	if (GetController() != NULL)
	{
		AddControllerYawInput(MouseLook.X);
		AddControllerPitchInput(MouseLook.Y);
	}
}

// Called every frame
void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInput)
	{
		EnhancedInput->BindAction(BirdMoveInput, ETriggerEvent::Triggered, this, &ABird::BirdMove);
		EnhancedInput->BindAction(BirdMoveInput, ETriggerEvent::Triggered, this, &ABird::BirdMoveRight);
		EnhancedInput->BindAction(BirdMouseInput, ETriggerEvent::Triggered, this, &ABird::BirdMouseLook);

		
	}
}

