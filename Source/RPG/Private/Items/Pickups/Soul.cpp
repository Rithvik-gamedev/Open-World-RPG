// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickups/Soul.h"
#include "Interfaces/PickupInterface.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void ASoul::BeginPlay()
{
	Super::BeginPlay();

	SoulDrift();
}

void ASoul::Tick(float DeltaTime)
{
	const double Location = GetActorLocation().Z;
	if (Location > DesiredZ)
	{
		const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
		AddActorWorldOffset(DeltaLocation, false);
	}
}


void ASoul::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* Interface =  Cast<IPickupInterface>(OtherActor);
	if (Interface)
	{
		Interface->AddSouls(this);
		SpawnPickupEffects();
		SpawnPickupSound();
		Destroy();
	}
	
	
}

void ASoul::SoulDrift()
{
	const FVector Start = GetActorLocation();
	const FVector End = GetActorLocation() - FVector(0.f, 0.f, 2000.f);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	ActorsToIgnore.AddUnique(GetOwner());
	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	DesiredZ = HitResult.ImpactPoint.Z + 125.f;
}
