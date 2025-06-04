// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class USoundBase;

enum class EItemStatus : uint8
{
	EIS_Equipped,
	EIS_Hovering
};

UCLASS()
class RPG_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AItem();
	virtual void Tick(float DeltaTime) override;

protected:
	
	virtual void BeginPlay() override;	
	
	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintPure)
	float SineFunction();

	virtual void SpawnPickupEffects();
	virtual void SpawnPickupSound();

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine variables")
	float Amplitude = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine variables")
	float TrigSpeed = 2.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	UNiagaraComponent* ItemEffects;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* PickupEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* PickupSound;

	EItemStatus ItemStatus = EItemStatus::EIS_Hovering;

private:

	float RunTime;
	
	

public:

	FORCEINLINE USphereComponent* GetSphere() { return Sphere; }
};
