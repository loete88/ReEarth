// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyType3.h"
#include "Player/VRHand/PlayerPawn.h"
#include "Player/PlayerRobot.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/AI/EnemyAIController.h"
#include "Enemy/Weapon/RocketBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

AEnemyType3::AEnemyType3()
{
	MaxHP = 200.0f;
	CurrentHP = MaxHP;
	
	EnableMove = false;

	//--------------------------------------------
	GetCapsuleComponent()->SetCapsuleHalfHeight(500);
	GetCapsuleComponent()->SetCapsuleRadius(300);

	//--------------------------------------------
	Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
	Tower->SetupAttachment(GetMesh());
	Tower->SetRelativeLocation(FVector(0, 0, -500));
	Tower->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>Tower_Asset(
		TEXT("/Game/AssetContents/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1"));
	if (Tower_Asset.Object)
	{
		Tower->SetStaticMesh(Tower_Asset.Object);
	}

	//--------------------------------------------
	Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	Gun->SetupAttachment(Tower);
	Gun->SetRelativeLocation(FVector(0, 0, 280));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>Gun_Asset(
		TEXT("/Game/AssetContents/CSC/Meshes/SK_CSC_Gun0.SK_CSC_Gun0"));
	if (Gun_Asset.Object)
	{
		Gun->SetSkeletalMesh(Gun_Asset.Object);
	}
}

float AEnemyType3::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP -= DamageAmount;
	UE_LOG(LogTemp, Log, TEXT("AEnemyType3 :: TakeDamage %f"), &CurrentHP);

	if (CurrentHP <= 0)
	{
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		Gun->SetRelativeLocation(FVector(50, 50, 280));
		Gun->AddRelativeRotation(FRotator(45, 0, 45));

		RemoveEnemy();
		IsDead = true;

		AEnemyAIController* AI = Cast<AEnemyAIController>(GetController());
		if (AI)
		{
			AI->IsDead(IsDead);
		}

		UE_LOG(LogTemp, Log, TEXT("AEnemyType3 :: IsDead True"));
	}

	return CurrentHP;
}


//------------------------------------------------------------------------------------
void AEnemyType3::AttackStart_Implementation()
{
	if (AttackTarget)
	{
		FTransform SocketT = Gun->GetSocketTransform(TEXT("Rocket"));
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->pRobot->GetActorLocation());
		//Rot.Roll = 0;
		//Rot.Pitch = 0;
		FVector Loc = SocketT.GetLocation() + UKismetMathLibrary::GetForwardVector(Rot) * 50;
		FTransform Trans = UKismetMathLibrary::MakeTransform(Loc, Rot, FVector(5.0f, 5.0f, 5.0f));

		GetWorld()->SpawnActor<AActor>(Rocket_Template, Trans);
	}
}

void AEnemyType3::AttackEnd_Implementation()
{
	AEnemyAIController* AI = Cast<AEnemyAIController>(GetController());
	if (AI)
	{
		AI->IsAttackEnd(true);
		ChangeState(EEnemyState::Idle);
	}
}

void AEnemyType3::RotateAttactTargetLoc_Implementation()
{
	if (AttackTarget)
	{
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
		//Gun->SetActorRotation(Rot);
		Gun->SetRelativeRotation(Rot);
	}
}
