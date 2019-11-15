// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHoming.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Enemy/Character/EnemyBase.h"

// Sets default values
APlayerHoming::APlayerHoming()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxComponent ���� �� ����
	//--------------------------------------------------------
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(76.0f, 26.0f, 21.0f));
	Box->SetCollisionProfileName(TEXT("NoCollision"));//ó���� NoCollision���� �߻��ϸ� Collision �����ֱ�
	Box->OnComponentBeginOverlap.AddDynamic(this, &APlayerHoming::DoActorBeginOverlap);
	//--------------------------------------------------------


	//Homing StaticMesh ���� �� ����
	//--------------------------------------------------------
	Homing = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Homing"));
	Homing->SetupAttachment(Box);
	Homing->SetRelativeScale3D(FVector(2.2f, 4.4f, 4.4f));
	Homing->SetCollisionProfileName(TEXT("NoCollision"));
	//--------------------------------------------------------


	//ProjectileMovment ���� �� ����
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

	//SetLifeSpan(4.0f);
}

// Called every frame
void APlayerHoming::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerHoming::Shot(AEnemyBase * Target)
{
	Box->SetCollisionProfileName(TEXT("SelfBullet"));
	pTarget = Target;
	
	//Shot�Լ� ȣ���
	//��ڿ� �����ִ� ������ ������ õõ�� ����.
	ProjectileMovement->MaxSpeed = 3000.0f;
	
	ProjectileMovement->Velocity = UKismetMathLibrary::GetForwardVector(Homing->K2_GetComponentRotation()) * 400;

	//Homing On �Լ� ȣ��
	HomingOn();
}

void APlayerHoming::DoActorBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogClass, Warning, TEXT("Actor Name : %s"), *OtherActor->GetName());
	UE_LOG(LogClass, Warning, TEXT("Comp Name : %s"),*OtherComp->GetName());

	

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollisionEffect, UKismetMathLibrary::MakeTransform(GetActorLocation(), FRotator(), FVector(4.0f)));

	SendDamage(OtherActor);

	Destroy();
}

void APlayerHoming::HomingOn()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HomingOnSound, GetActorLocation());

	//EnemyTarget�� ��ǥ�� ����
	ProjectileMovement->HomingTargetComponent = pTarget->HomingPosition;


	FTransform ParticleTransform = UKismetMathLibrary::MakeTransform(FVector(),
		FRotator(),
		FVector(3.0f));

	//Trail�� ���� �����ϰ� �����ִ� ����
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &APlayerHoming::HomingTrail, 1.0f, false);
}

void APlayerHoming::HomingTrail()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), TrailSound, GetActorLocation());

	UGameplayStatics::SpawnEmitterAttached(TrailEffect, Homing,TEXT("HomingEnd"));

	ProjectileMovement->HomingAccelerationMagnitude = 5000.0f;
}

void APlayerHoming::SendDamage(AActor * OtherActor)
{
	UGameplayStatics::ApplyDamage(OtherActor, 100.0f,
		UGameplayStatics::GetPlayerController(GetWorld(), 0), OtherActor, NULL);
}

