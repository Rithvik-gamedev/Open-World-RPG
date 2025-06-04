// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon/Weapon.h"
#include "Characters/RPG_Character.h"
#include "Characters/CharacterTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneCaptureComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "Interfaces/HitInterface.h"
#include "NiagaraFunctionLibrary.h"


AWeapon::AWeapon()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	BoxComponent->SetupAttachment(RootComponent);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Start Location"));
	BoxTraceStart->SetupAttachment(BoxComponent);

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("End Location"));
	BoxTraceEnd->SetupAttachment(BoxComponent);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxBeginOverlap);
	InitialRotation = GetActorRotation();
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsActorSameType(OtherActor)) return;

	FHitResult HitResult;
	BoxTrace(HitResult);

	if (HitResult.GetActor())
	{
		if (IsActorSameType(HitResult.GetActor())) return;

		UGameplayStatics::ApplyDamage(HitResult.GetActor(), WeaponDamage, GetInstigatorController(), this, UDamageType::StaticClass());
		ExecuteGetHit(HitResult);
		CreateFieldSystemBreakObjects(HitResult.ImpactPoint);
	}
}

bool AWeapon::IsActorSameType(AActor* OtherActor)
{
	return  GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::ExecuteGetHit(FHitResult& HitResult)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint, GetOwner());
	}
}

void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector StartLocation = BoxTraceStart->GetComponentLocation();
	const FVector EndLocation = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actors : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actors);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		StartLocation,
		EndLocation,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());
}

void AWeapon::EquipWeapon(USceneComponent* InParent, FName InSocket, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocket);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ItemStatus = EItemStatus::EIS_Equipped;
	
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation()
			);
	}
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void AWeapon::WeaponDetach()
{
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorRotation(InitialRotation);
	ItemStatus = EItemStatus::EIS_Hovering;

}


void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocket)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocket);
}


