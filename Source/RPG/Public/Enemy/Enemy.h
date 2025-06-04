// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/BaseCharacter.h"
#include "Characters/CharacterTypes.h"
#include "Enemy.generated.h"

class UWidgetComponent;
class UHealthBarComponent;
class UPawnSensingComponent;

UCLASS()
class RPG_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	/* <AActor>*/
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/* </AActor>*/

	/* <IHitInterface>*/
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/* </IHitInterface>*/

protected:

	/* <AActor>*/
	virtual void BeginPlay() override;
	/* </AActor>*/

	/* <ABaseCharacter>*/
	virtual void Die() override;
	void SpawnSoul();
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	virtual void PlayAttackMontage() override;
	virtual void EndAttack() override;
	/* </ABaseCharacter>*/

	void Attack();

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;

private:
	/* AI Behavior*/
	void InitializeEnemy();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseIntrest();
	void StartPatrolling();
	void ChaseTarget();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsDead();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();
	void SpawnDefaultWeapon();

	UFUNCTION()
	void PawnSeen(APawn* Pawn); // Callback for OnPawnSeen in UPawnSensingComponent

	FTimerHandle PatrolTimer;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation", BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AActor* PatrolTarget;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	double PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 5.f;

	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 10.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float PatrollingSpeed = 125.f;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;

	FTimerHandle AttackTimer;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double AttackRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	double AcceptanceRadius = 70.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float ChasingSpeed = 300.f;

	UPROPERTY(VisibleAnywhere, Category = "AI Navigation")
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UHealthBarComponent* HealthWidget;


	UPROPERTY(Editanywhere, Category = "Combat")
	float DeathLifeSpan = 5.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<class ASoul> SoulClass;

};
