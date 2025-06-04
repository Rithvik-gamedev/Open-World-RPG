// Fill out your copyright notice in the Description page of Project Settings.


#include "GeometryBreakables/BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "Items/Pickups/Treasure.h"

// Sets default values
ABreakableActor::ABreakableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BreakableObject = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("BreakableObject"));
	SetRootComponent(BreakableObject);
	BreakableObject->SetGenerateOverlapEvents(true);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleCollision"));
	Capsule->SetupAttachment(RootComponent);
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	Capsule->SetGenerateOverlapEvents(false);

}

// Called when the game starts or when spawned
void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
	BreakableObject->OnChaosBreakEvent.AddDynamic(this, &ABreakableActor::FOnChaosBreakEvent);
}

void ABreakableActor::FOnChaosBreakEvent(const FChaosBreakEvent& BreakEvent)
{
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	FVector ImpactLocation = BreakEvent.Location;
	GetHit_Implementation(ImpactLocation, GetOwner());
	this->SetLifeSpan(3.f);
}


// Called every frame
void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (bBroken) return;
	bBroken = true;
	UWorld* World = GetWorld();
	FVector Location = GetActorLocation();
	Location.Z += TreasureSpawnHeight;
	if (World && TreasureClasses.Num() > 0)
	{
		int32 Selection = FMath::RandRange(0, TreasureClasses.Num() - 1);
		ATreasure* SpawnedActor = World->SpawnActor<ATreasure>(TreasureClasses[Selection], Location, GetActorRotation());
		
		if (SpawnedActor)
		{
			
			SpawnedActor->GetSphere()->SetGenerateOverlapEvents(false);

			FTimerHandle TimerHandle; 
			FTimerDelegate TimerDelegate;

			TimerDelegate.BindLambda([SpawnedActor]()
			{
				if (SpawnedActor && SpawnedActor->GetSphere()) 
				{ 
					SpawnedActor->GetSphere()->SetGenerateOverlapEvents(true); 
				}
			});

			World->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, false);
		}
	}
}

