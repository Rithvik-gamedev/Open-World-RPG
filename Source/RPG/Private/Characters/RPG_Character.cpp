// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RPG_Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GroomComponent.h"
#include "Items/Item.h"
#include "Items/Weapon/Weapon.h"
#include "Items/Pickups/Soul.h"
#include "Items/Pickups/Treasure.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPG_Components/Attributes.h"
#include "HUD/SlashHUD.h"
#include "HUD/PlayerOverlay.h"

#include "Components/InputComponent.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputTriggers.h"


ARPG_Character::ARPG_Character()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	CharSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CharSpringArm->SetupAttachment(RootComponent);
	CharSpringArm->bUsePawnControlRotation = true;

	CharCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Character Camera"));
	CharCamera->SetupAttachment(CharSpringArm);

	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Hair->AttachmentName = FString("head");

	Eyebrow = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrow->SetupAttachment(GetMesh());
	Eyebrow->AttachmentName = FString("head");
}

void ARPG_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bIsFalling = GetCharacterMovement()->IsFalling();

	RegenStamina(DeltaTime);

}

void ARPG_Character::RegenStamina(float DeltaTime)
{
	if (BaseAttributes && PlayerOverlay)
	{
		
		BaseAttributes->RegenStamina(DeltaTime);
		PlayerOverlay->SetStaminaBarProgress(BaseAttributes->GetStaminaPercent());
	}
}

void ARPG_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	UInputTriggerHold* HoldTrigger = NewObject<UInputTriggerHold>(this);
	HoldTrigger->HoldTimeThreshold = 0.5f;


	if (EnhancedInput)
	{
		EnhancedInput->BindAction(CharacterMoveIA, ETriggerEvent::Triggered, this, &ARPG_Character::CharMove);
		EnhancedInput->BindAction(CharacterLookIA, ETriggerEvent::Triggered, this, &ARPG_Character::CharLook);
		EnhancedInput->BindAction(CharInteractIA, ETriggerEvent::Triggered, this, &ARPG_Character::CharInteract);
		EnhancedInput->BindAction(JumpIA, ETriggerEvent::Triggered, this, &ARPG_Character::CharJump);
		EnhancedInput->BindAction(AttackIA, ETriggerEvent::Triggered, this, &ARPG_Character::Attack);
		EnhancedInput->BindAction(DropItemIA, ETriggerEvent::Triggered, this, &ARPG_Character::DropItem);
		EnhancedInput->BindAction(DodgeIA, ETriggerEvent::Triggered, this, &ARPG_Character::Dodge);
	
	}
}

void ARPG_Character::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint, Hitter);
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (BaseAttributes && BaseAttributes->IsAlive())
	{
		ActionState = EActionState::EAS_HitReacting;
	}
}

void ARPG_Character::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void ARPG_Character::AddSouls(ASoul* Soul)
{
	if (BaseAttributes && PlayerOverlay)
	{
		BaseAttributes->AddSouls(Soul->GetSouls());
		PlayerOverlay->SetSoulsText(BaseAttributes->GetSouls());
	}
}

void ARPG_Character::AddGold(ATreasure* Gold)
{
	if (BaseAttributes && PlayerOverlay)
	{
		BaseAttributes->AddGold(Gold->GetGold());
		PlayerOverlay->SetGoldText(BaseAttributes->GetGold());
	}
}

void ARPG_Character::SetHealthProgress()
{
	if (BaseAttributes && PlayerOverlay)
	{
		PlayerOverlay->SetHealthBarProgress(BaseAttributes->GetHealthPercent());
	}
}

void ARPG_Character::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("EngageableCharacter"));
	
	APlayerController* CharController = Cast<APlayerController>(GetController());

	if (CharController)
	{
		/*Setup MappingContext*/
		UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(CharController->GetLocalPlayer());
		if (SubSystem)
		{
			SubSystem->AddMappingContext(CharacterMappingcontext, 0);
		}

		InitializePlayerOverlay(CharController);
	}
}

void ARPG_Character::CharMove(const FInputActionInstance& Value)
{
	const FVector2D VectorValue = Value.GetValue().Get<FVector2D>();
	
	if (!IsUnoccupied()) return;

	if (GetController())
	{

		const FRotator ControllerRotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0.f, ControllerRotation.Yaw, 0.f);

		FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, VectorValue.Y);
		AddMovementInput(RightDirection, VectorValue.X);

	}
}

void ARPG_Character::CharLook(const FInputActionInstance& Value)
{
	const FVector2D LookVector = Value.GetValue().Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookVector.X);
		AddControllerPitchInput(LookVector.Y);
	}
}

void ARPG_Character::CharJump(const FInputActionInstance& Value)
{
	if (IsUnoccupied())
	{
		Jump();
	}
}


void ARPG_Character::CharInteract(const FInputActionInstance& Value)
{

	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		if (EquippedWeapon)
		{
			Dropweapon();
			EquipWeapon(OverlappingWeapon);
			
		}
		else EquipWeapon(OverlappingWeapon);
			
	}
	else
	{
		if (CanDisarm() && !bIsFalling)
		{
			DisArm();
		}
		else if (CanArm() && !bIsFalling)
		{
			
			Arm();
		}
	}
}

void ARPG_Character::EquipWeapon(AWeapon* Weapon)
{
	FName SocketName = FName();

	//Checking if the weapon type is two handed
	if (Weapon->bIsTwoHanded)
	{
		SocketName = "RightTwoHandedSocket";
		CharacterState = ECharacterState::ECS_EquippedTwoHanded;
	}
	else
	{
		SocketName = "RightHandWeaponSocket";
		CharacterState = ECharacterState::ECS_Equipped;
	}
	//Attaching Weapon to respective socket
	Weapon->EquipWeapon(GetMesh(), FName(SocketName), this, this);
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void ARPG_Character::Attack(const FInputActionInstance& Value)
{
	bool boolValue = Value.GetValue().Get<bool>();
	if (GetController() && boolValue)
	{
		if (bIsFalling) return;

		if (CanAttack())
		{
			if (BaseAttributes && BaseAttributes->GetStamina() > StaminaCostAttack)
			{
				BaseAttributes->UseStamina(StaminaCostAttack);
				PlayAttackMontage();
				ActionState = EActionState::EAS_Attacking;
			}
			else return;
		}
	}
}

void ARPG_Character::DropItem(const FInputActionInstance& Value)
{
	bool boolValue = Value.GetValue().Get<bool>();
	if (GetController() && boolValue)
	{
		Dropweapon();
		CharacterState = ECharacterState::ECS_Unequipped;
		ActionState = EActionState::EAS_EquippingWeapon;
	}
	ActionState = EActionState::EAS_Unoccupied;
}

void ARPG_Character::Dodge(const FInputActionInstance& Value)
{
	bool bValue = Value.GetValue().Get<bool>();
	if (GetController() && bValue)
	{
		if (!IsUnoccupied()) return;
		
		if (BaseAttributes && BaseAttributes->GetStamina() > StaminaCostDodge && PlayerOverlay)
		{
			PlayDodgeMontage();
			BaseAttributes->UseStamina(StaminaCostDodge);
			PlayerOverlay->SetStaminaBarProgress(BaseAttributes->GetStaminaPercent());
			ActionState = EActionState::EAS_Dodging;
		}
	}
}

bool ARPG_Character::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && (CharacterState == ECharacterState::ECS_Equipped or CharacterState == ECharacterState::ECS_EquippedTwoHanded);
}

bool ARPG_Character::IsTwoHanded()
{

	return EquippedWeapon->bIsTwoHanded;

}

bool ARPG_Character::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

bool ARPG_Character::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool ARPG_Character::CanDropWeapon()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

void ARPG_Character::EndAttack()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ARPG_Character::HandleDamage(float DamageAmount)
{
	if (BaseAttributes)
	{
		BaseAttributes->ReceiveDamage(DamageAmount);
	}
}

float ARPG_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHealthHUD();
	return DamageAmount;
}

void ARPG_Character::Die()
{
	Super::Die();

	ActionState = EActionState::EAS_Dead;
	GetMesh()->SetGenerateOverlapEvents(false);
	DisableMeshCollision();
	
}

void ARPG_Character::PlayAttackMontage()
{
	if (IsTwoHanded())
	{
		PlayTwoHandedAttackMontage();
	}
	else Super::PlayAttackMontage();
}

void ARPG_Character::PlayDodgeMontage()
{
	Super::PlayDodgeMontage();
	
}

void ARPG_Character::PlayUnEquipMontage()
{
	LastSelection = PlayRandomMontageSection(UnEquipMontage, UnEquipMontageSections);
}
void ARPG_Character::PlayEquipMontage()
{
	PlayMontageSection(EquipMontage, EquipMontageSections[LastSelection]);
}

void ARPG_Character::PlayTwoHandedEquipMontage()
{
	PlayRandomMontageSection(TwoHandedEquipMontage, TwoHandedEquipMontageSections);
}

void ARPG_Character::PlayTwoHandedUnEquipMontage()
{
	PlayRandomMontageSection(TwoHandedUnEquipMontage, TwoHandedUnEquipMontageSections);
}

void ARPG_Character::Arm()
{
	if (IsTwoHanded())
	{
		PlayTwoHandedEquipMontage();
		CharacterState = ECharacterState::ECS_EquippedTwoHanded;
	}
	else
	{
		PlayEquipMontage();
		CharacterState = ECharacterState::ECS_Equipped;
	}
	ActionState = EActionState::EAS_EquippingWeapon;
	
}

void ARPG_Character::DisArm()
{
	if (IsTwoHanded())
	{
		PlayTwoHandedUnEquipMontage();
	}
	else PlayUnEquipMontage();
	CharacterState = ECharacterState::ECS_Unequipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void ARPG_Character::AttachWeapontoHand()
{
	if (EquipMontage == nullptr && EquippedWeapon == nullptr) return;

	if (IsTwoHanded())
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightTwoHandedSocket"));
	}
	else
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandWeaponSocket"));
	}
}

void ARPG_Character::AttachWeapontoSpine()
{
	if (UnEquipMontage && EquippedWeapon)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		FName SectionName = AnimInstance->Montage_GetCurrentSection(UnEquipMontage);
		if (EquippedWeapon->bIsTwoHanded)
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineTwoHandedSocketOver"));
		}
		else if (SectionName == FName("UnEquip1"))
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineWeaponSocketOver"));
		}
		else
		{
			EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineWeaponSocketUnder"));
		}
	}
}

void ARPG_Character::Dropweapon()
{
	if (EquippedWeapon && CanDropWeapon())
	{
		EquippedWeapon->WeaponDetach();
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		EquippedWeapon = nullptr;

	}
}

void ARPG_Character::EndEquipOrUnequipNotify()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ARPG_Character::EndHitReact()
{
	ActionState = EActionState::EAS_Unoccupied;
}

void ARPG_Character::EndDodge()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ARPG_Character::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

bool ARPG_Character::IsDodging()
{
	return ActionState == EActionState::EAS_Dodging;
}

void ARPG_Character::InitializePlayerOverlay(APlayerController* CharController)
{
	ASlashHUD* SlashHUD = Cast<ASlashHUD>(CharController->GetHUD());
	if (SlashHUD)
	{
		PlayerOverlay = SlashHUD->GetSlashPlayerOverlay();
		if (PlayerOverlay)
		{
			PlayerOverlay->SetHealthBarProgress(BaseAttributes->GetHealthPercent());
			PlayerOverlay->SetStaminaBarProgress(BaseAttributes->GetStaminaPercent());
			PlayerOverlay->SetGoldText(BaseAttributes->GetGold());
			PlayerOverlay->SetSoulsText(BaseAttributes->GetSouls());
		}
	}
}

void ARPG_Character::SetHealthHUD()
{
	if (PlayerOverlay && BaseAttributes)
	{
		PlayerOverlay->SetHealthBarProgress(BaseAttributes->GetHealthPercent());
	}
}

