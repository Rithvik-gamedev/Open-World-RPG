// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Pickups/Treasure.h"
#include "Characters/RPG_Character.h"


ATreasure::ATreasure()
{

}

void ATreasure::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickupInterface* Interface = Cast<IPickupInterface>(OtherActor);
	if (Interface)
	{	
		Interface->AddGold(this);
		SpawnPickupSound();
		Destroy();
	}
	
}	

void ATreasure::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}
