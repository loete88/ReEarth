// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyType3.h"
#include "Player/VRHand/PlayerPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/AI/EnemyAIController.h"
#include "Enemy/Weapon/RocketBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"

AEnemyType3::AEnemyType3()
{
	EnableMove = false;

	//--------------------------------------------
	GetCapsuleComponent()->SetCapsuleHalfHeight(500);
	GetCapsuleComponent()->SetCapsuleRadius(200);

	//--------------------------------------------
	Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tower"));
	Tower->SetupAttachment(RootComponent);
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

//------------------------------------------------------------------------------------
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
