// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Attributes.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UAttributes : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttributes();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = ture))
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = ture))
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = ture))
	float Stamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = ture))
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = ture))
	float StaminaRegenRate = 6.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = ture))
	int32 Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Actor Attributes", meta = (AllowPrivateAccess = ture))
	int32 Souls;


public:

	FORCEINLINE float GetMaxHealth() const { return MaxHealth;}
	FORCEINLINE float GetStamina() const { return Stamina; }
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }
	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetSouls() const { return Souls; }
	void AddSouls(int32 NumberOfSouls);
	void AddGold(int32 AmountOfGold);

	void ReceiveDamage(float Damage);
	void UseStamina(float StaminaAmount);
	void RegenStamina(float DeltaTime);
	float GetHealthPercent();
	float GetStaminaPercent();
	bool IsAlive();




		
};
