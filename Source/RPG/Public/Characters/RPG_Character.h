// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "InputAction.h"
#include "Characters/CharacterTypes.h"
#include "Interfaces/PickupInterface.h"
#include "RPG_Character.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
class UGroomComponent;
class UAnimMontage;
class UPlayerOverlay;
class AItem;

UCLASS()
class RPG_API ARPG_Character : public ABaseCharacter, public IPickupInterface
{
	GENERATED_BODY()

public:
	ARPG_Character();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void SetOverlappingItem(AItem* Item) override;
	virtual void AddSouls(ASoul* Soul) override;
	virtual void AddGold(ATreasure* Gold) override;

	UFUNCTION(BlueprintCallable)
	void SetHealthProgress();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	/*Character Input Callback*/
	void CharMove(const FInputActionInstance& Value);
	void CharLook(const FInputActionInstance& Value);
	void CharJump(const FInputActionInstance& Value);
	void CharInteract(const FInputActionInstance& Value);
	void Attack(const FInputActionInstance& Value);
	void DropItem(const FInputActionInstance& Value);
	void Dodge(const FInputActionInstance& Value);
	
	/**Combat*/
	virtual bool CanAttack() override;
	bool IsTwoHanded();
	bool CanArm();
	bool CanDisarm();
	bool CanDropWeapon();
	void EquipWeapon(AWeapon* Weapon);
	virtual void EndAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die() override;
	void RegenStamina(float Deltatime);

	/*Play Montages*/
	virtual void PlayAttackMontage() override;
	virtual void PlayDodgeMontage() override;
	void PlayUnEquipMontage();
	void PlayEquipMontage();
	void PlayTwoHandedEquipMontage();
	void PlayTwoHandedUnEquipMontage();
	void Arm();
	void DisArm();

	UFUNCTION(BlueprintCallable)
	void AttachWeapontoHand();

	UFUNCTION(BlueprintCallable)
	void AttachWeapontoSpine();

	UFUNCTION(BlueprintCallable)
	void Dropweapon();

	UFUNCTION(BlueprintCallable)
	void EndEquipOrUnequipNotify();

	UFUNCTION(BlueprintCallable)
	void EndHitReact();

	UFUNCTION(BlueprintCallable)
	void EndDodge();
	

private:

	bool IsUnoccupied();
	bool IsDodging();
	void InitializePlayerOverlay(APlayerController* CharController);
	void SetHealthHUD();


	UPROPERTY(EditAnywhere, Category = "Character Input")
	UInputMappingContext* CharacterMappingcontext;

	UPROPERTY(EditAnywhere, Category = "Character Input")
	UInputAction* CharacterMoveIA;

	UPROPERTY(EditAnywhere, Category = "Character Input")
	UInputAction* CharacterLookIA;

	UPROPERTY(EditAnywhere, Category = "Character Input")
	UInputAction* CharInteractIA;

	UPROPERTY(EditAnywhere, Category = "Character Input")
	UInputAction* JumpIA;

	UPROPERTY(EditAnywhere, Category = "Character Input")
	UInputAction* AttackIA;

	UPROPERTY(EditAnywhere, Category = "Character Input")
	UInputAction* DropItemIA;

	UPROPERTY(EditAnywhere, Category = "Character Input")
	UInputAction* DodgeIA;

	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* TwoHandedEquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* UnEquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* TwoHandedUnEquipMontage;

	/* For Equip and Unequip Selections*/
	int32 LastSelection = 0;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> EquipMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> UnEquipMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> TwoHandedEquipMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> TwoHandedUnEquipMontageSections;


	UPROPERTY(EditAnywhere, Category = "Combat")
	double MinDistanceLockOn = 750.f;

	TArray<AActor*> CombatTargetsInRange;
	
	/**Character Components*/

	bool bIsFalling;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CharSpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CharCamera;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* Eyebrow;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;

	UPROPERTY(BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowprivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float StaminaCostDodge = 20.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float StaminaCostAttack = 10.f;

	FTimerHandle DodgeTimer;

public:

	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }

	FORCEINLINE EActionState GetActionState() const { return ActionState; }

};
