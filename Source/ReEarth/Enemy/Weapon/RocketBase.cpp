// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"

ARocketBase::ARocketBase()
{
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(35.0f, 10.0f, 10.0f));
	Box->SetCollisionProfileName(TEXT("EnemyBullet"));
	Box->OnComponentBeginOverlap.AddDynamic(this, &ARocketBase::DoActorBeginOverlap);

	Rocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Rocket"));
	Rocket->SetupAttachment(Box);
	Rocket->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	Rocket->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	Rocket->SetCollisionProfileName(TEXT("NoCollision"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(
		TEXT("/Game/AssetContents/CombatMissilePack/Meshes/Missile_AirToSurface_AGM114.Missile_AirToSurface_AGM114"));
	UStaticMesh* Asset = MeshAsset.Object;
	if (Asset)
	{
		Rocket->SetStaticMesh(Asset);
	}
	
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 3000.0f;
	Movement->MaxSpeed = 3000.0f;
	Movement->ProjectileGravityScale = 0.01f;
}

void ARocketBase::DoActorBeginOverlap(UPrimitiveComponent * OverlappedComp, 
	AActor * OtherActor, UPrimitiveComponent * OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("ARocketBase :: DoActorBeginOverlap"));
}

