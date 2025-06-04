// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Hitinterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class UCapsuleComponent;
class UNiagaraComponent;

UCLASS()
class RPG_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void FOnChaosBreakEvent(const FChaosBreakEvent& BreakEvent);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGeometryCollectionComponent* BreakableObject;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCapsuleComponent* Capsule;
	

private:

	UPROPERTY(EditDefaultsOnly, Category = "Breakable Properties")
	TArray<TSubclassOf<class ATreasure>> TreasureClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Breakable Properties")
	float TreasureSpawnHeight = 50.f;

	bool bBroken = false;


};
