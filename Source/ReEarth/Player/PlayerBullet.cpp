// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"



// Sets default values
APlayerBullet::APlayerBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Box 설정
	//---------------------------------------------------------
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(54.0f, 10.0f, 10.0f));
	Box->SetCollisionProfileName(TEXT("SelfBullet"));
	Box->OnComponentBeginOverlap.AddDynamic(this, &APlayerBullet::DoActorBeginOverlap);
	//---------------------------------------------------------
	
	//Mesh 설정
	//---------------------------------------------------------
	Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
	Bullet->SetupAttachment(Box);
	Bullet->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Bullet->SetWorldScale3D(FVector(8.0f, 8.0f, 9.0f));
	Bullet->SetCollisionProfileName(TEXT("NoCollision"));
	//---------------------------------------------------------

	//Projectile설정
	//------------------------------------------------------------------------------
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 8000.0f;
	Movement->MaxSpeed = 8000.0f;
	Movement->ProjectileGravityScale = 0.01f;
	//------------------------------------------------------------------------------

}

// Called when the game starts or when spawned
void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();


	UGameplayStatics::SpawnEmitterAttached(Trail, Bullet,TEXT("BulletEnd"));

	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Trail, GetActorLocation(),FRotator::ZeroRotator,9.0f);

	SetLifeSpan(3.0f);
	
}

// Called every frame
void APlayerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerBullet::DoActorBeginOverlap(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//여기에 들어가야하는 것
	//Enemy클래스로 형변환해서 성공하면 SpawnEmitterAt location 호출한다.
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollisionEffect, UKismetMathLibrary::MakeTransform(GetActorLocation(), FRotator(), FVector(4.0f)));

	SendDamage(OtherActor);

	Destroy();
}

void APlayerBullet::SendDamage(AActor * OtherActor)
{
	UGameplayStatics::ApplyDamage(OtherActor, PlayerBulletDamage
		,UGameplayStatics::GetPlayerController(GetWorld(),0), OtherActor, NULL);
}

