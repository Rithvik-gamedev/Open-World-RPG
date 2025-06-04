// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapon/Weapon.h"
#include "RPG_Components/Attributes.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"


ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseAttributes = CreateDefaultSubobject<UAttributes>(TEXT("Attributes"));

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive())
	{
		DirectionalHitReact(Hitter->GetActorLocation());
	}
	else Die();

	PlayHitSound(ImpactPoint);
	PlayHitParticle(ImpactPoint);
}

void ABaseCharacter::Die()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
	
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

void ABaseCharacter::PlayAttackMontage()
{
	PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

void ABaseCharacter::PlayTwoHandedAttackMontage()
{
	PlayRandomMontageSection(TwoHandedAttackMontage, TwoHandedAttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_DefaultMax)
	{
		DeathPoseState = Pose;
	}
	return Selection;
}

void ABaseCharacter::PlayDodgeMontage()
{
	if (DodgeMontage)
	{
		if (GetCharacterMovement()->IsFalling())
		{
			PlayMontageSection(DodgeMontage, FName(TEXT("InAir")));
		}
		else
		{
			PlayMontageSection(DodgeMontage, FName(TEXT("Ground")));
		}
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionName)
{
	if (Montage == nullptr && SectionName.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionName.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionName[Selection]);
	return Selection;
}

FVector ABaseCharacter::GetTranslationWarpTarget()
{
	if(CombatTarget == nullptr) return FVector();

	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();

	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;

	return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget()
{
	if (CombatTarget)
	{
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	//Calculate DotProduct between forward vector and vector from enemy location to hit location to get the cos angle
	// ForwardVector * ToHit = |Forward| |ToHit| Cos(Theta)
	const FVector  ForwardVector = GetActorForwardVector();
	const FVector ToHit = (ImpactPoint - GetActorLocation()).GetSafeNormal2D();
	double CosTheta = FVector::DotProduct(ForwardVector, ToHit);

	//getting the angle using arc-cosine
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// If cross product is pointing downwards Theta is negative
	FVector CrossProduct = FVector::CrossProduct(ForwardVector, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
}

void ABaseCharacter::PlayHitParticle(const FVector& ImpactPoint)
{
	if (HitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, ImpactPoint);
	}
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (BaseAttributes)
	{
		BaseAttributes->ReceiveDamage(DamageAmount);
	}
}

bool ABaseCharacter::IsAlive()
{
	return BaseAttributes && BaseAttributes->IsAlive();
}

void ABaseCharacter::EndAttack()
{

}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabledType)
{
	if (EquippedWeapon && EquippedWeapon->GetBoxComponent())
	{
		EquippedWeapon->GetBoxComponent()->SetCollisionEnabled(CollisionEnabledType);
		EquippedWeapon->IgnoreActors.Empty();
		
	}
}

void ABaseCharacter::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
