// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHoming.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
APlayerHoming::APlayerHoming()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxComponent 생성 및 설정
	//--------------------------------------------------------
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetBoxExtent(FVector(76.0f, 26.0f, 21.0f));
	Box->SetCollisionProfileName(TEXT("SelfBullet"));
	//--------------------------------------------------------


	//Homing StaticMesh 생성 및 설정
	//--------------------------------------------------------
	Homing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Homing"));
	Homing->SetupAttachment(Box);
	Homing->SetRelativeScale3D(FVector(2.2f, 4.4f, 4.4f));
	Homing->SetCollisionProfileName(TEXT("NoCollision"));
	//--------------------------------------------------------


	//ProjectileMovment 생성 및 설정
	//--------------------------------------------------------
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ProjectileMovement->InitialSpeed = 0.0f;
	ProjectileMovement->MaxSpeed = 5000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->Velocity = FVector(0.0f, 0.0f, 0.0f);
	//--------------------------------------------------------
}

// Called when the game starts or when spawned
void APlayerHoming::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerHoming::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerHoming::Shot(AActor * Target)
{
}

