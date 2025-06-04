// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Components/Attributes.h"


UAttributes::UAttributes()
{
	
	PrimaryComponentTick.bCanEverTick = false;

}


void UAttributes::BeginPlay()
{
	Super::BeginPlay();

}

void UAttributes::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

void UAttributes::AddSouls(int32 NumberOfSouls)
{
	Souls += NumberOfSouls;
}

void UAttributes::AddGold(int32 AmountOfGold)
{
	Gold += AmountOfGold;
}

void UAttributes::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

void UAttributes::UseStamina(float StaminaAmount)
{
	Stamina = FMath::Clamp(Stamina - StaminaAmount, 0.f, MaxStamina);
}

float UAttributes::GetHealthPercent()
{
	return Health/MaxHealth;
}

float UAttributes::GetStaminaPercent()
{
	return Stamina/MaxStamina;
}

bool UAttributes::IsAlive()
{
	return Health > 0.f;
}

void UAttributes::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

