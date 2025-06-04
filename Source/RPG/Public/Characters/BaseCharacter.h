// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "Characters/CharacterTypes.h"
#include "BaseCharacter.generated.h"

class AWeapon;
class UAttributes;
class UAnimMontage;
class UBaseSound;
class UParticleSystem;

UCLASS()
class RPG_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;
	

protected:
	virtual void BeginPlay() override;
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void Die();
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void PlayHitSound(const FVector& ImpactPoint);
	void PlayHitParticle(const FVector& ImpactPoint);
	virtual bool CanAttack();
	bool IsAlive();

	/**Montages*/
	void PlayHitReactMontage(const FName& SectionName);
	virtual void PlayAttackMontage();
	virtual void StopAttackMontage();
	virtual void PlayTwoHandedAttackMontage();
	virtual int32 PlayDeathMontage();
	virtual void PlayDodgeMontage();
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionName);
	
	void DisableMeshCollision();
	
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget();

	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget();

	UFUNCTION(BlueprintCallable)
	virtual void EndAttack();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabledType);


	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 500.f;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* EquippedWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAttributes* BaseAttributes;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double WarpTargetDistance = 75.f;


	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EDeathPose> DeathPoseState;


private:

	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	UParticleSystem* HitParticle;

	/*Animation Montages*/
	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* TwoHandedAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Character Montages")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> TwoHandedAttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FName> DeathMontageSections;

public:
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPoseState;  }
	FORCEINLINE UAttributes* GetBaseAttributes() const { return BaseAttributes; }
	
};
