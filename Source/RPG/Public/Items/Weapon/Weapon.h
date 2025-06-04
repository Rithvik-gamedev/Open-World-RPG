// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapon.generated.h"

/**
 * 
 */

class USoundBase;
class UBoxComponent;
class USceneComponent;

UCLASS()
class RPG_API AWeapon : public AItem
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay();

	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsActorSameType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& HitResult);

	//void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void CreateFieldSystemBreakObjects(const FVector& FieldLocation);

	/***************************************************/



public:

	AWeapon();

	void EquipWeapon(USceneComponent* InParent, FName InSocket, AActor* NewOwner, APawn* NewInstigator);
	void WeaponDetach();

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocket);

	TArray<AActor*> IgnoreActors;

private:

	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f, 5.f, 5.f);
	
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere, Category = "Box Trace Properties")
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Box Trace Properties")
	USceneComponent* BoxTraceStart;

	UPROPERTY(EditDefaultsOnly, Category = "Box Trace Properties")
	USceneComponent* BoxTraceEnd;

	FRotator InitialRotation;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float WeaponDamage = 25.f;

	

public:

	UBoxComponent* GetBoxComponent() const { return BoxComponent; }

	UPROPERTY(EditAnywhere)
	bool bIsTwoHanded = false;
};
