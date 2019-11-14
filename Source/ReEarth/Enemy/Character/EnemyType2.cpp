// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyType2.h"
#include "Player/VRHand/PlayerPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/AI/EnemyAIController.h"
#include "Enemy/Weapon/RocketBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyType2::AEnemyType2()
{
	MaxHP = 100.0f;
	CurrentHP = MaxHP;

	//--------------------------------------------
	GetCapsuleComponent()->SetCapsuleHalfHeight(120);
	GetCapsuleComponent()->SetCapsuleRadius(120);

	//--------------------------------------------
	GetMesh()->SetRelativeLocation(FVector(0, 0, -120));
	GetMesh()->SetRelativeRotation(FRotator(0, 0, 0));
	GetMesh()->SetWorldScale3D(FVector(0.7f, 0.7f, 0.7f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(
		TEXT("/Game/AssetContents/CSC/Meshes/SK_CSC_Base1.SK_CSC_Base1"));
	if (MeshAsset.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}

	//--------------------------------------------
	SK_CSC_Body1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_CSC_Body1"));
	SK_CSC_Body1->SetupAttachment(GetMesh());
	SK_CSC_Body1->SetRelativeLocation(FVector(0.130047, 0.00012, 84.807945));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_CSC_Body1_Asset(
		TEXT("/Game/AssetContents/CSC/Meshes/SK_CSC_Body1.SK_CSC_Body1"));
	if (SK_CSC_Body1_Asset.Object)
	{
		SK_CSC_Body1->SetSkeletalMesh(SK_CSC_Body1_Asset.Object);
	}
	//--------------------------------------------
	SK_CSC_1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_CSC_1"));
	SK_CSC_1->SetupAttachment(SK_CSC_Body1);
	SK_CSC_1->SetRelativeLocation(FVector(-97.846138, 1.500416, 40.8349));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_CSC_1_Asset(
		TEXT("/Game/AssetContents/CSC/Meshes/SK_CSC_Device2.SK_CSC_Device2"));
	if (SK_CSC_1_Asset.Object)
	{
		SK_CSC_1->SetSkeletalMesh(SK_CSC_1_Asset.Object);
	}

	//--------------------------------------------
	SK_CSC_Gun1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_CSC_Gun1"));
	SK_CSC_Gun1->SetupAttachment(SK_CSC_Body1);
	SK_CSC_Gun1->SetRelativeLocation(FVector(-0.682, -21.73045, 107.940933));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_CSC_Gun1_Asset(
		TEXT("/Game/AssetContents/CSC/Meshes/SK_CSC_Gun5.SK_CSC_Gun5"));
	if (SK_CSC_Gun1_Asset.Object)
	{
		SK_CSC_Gun1->SetSkeletalMesh(SK_CSC_Gun1_Asset.Object);
	}

	//--------------------------------------------
	SK_CSC_Gun2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_CSC_Gun2"));
	SK_CSC_Gun2->SetupAttachment(SK_CSC_Body1);
	SK_CSC_Gun2->SetRelativeLocation(FVector(-0.682, 21.73045, 107.940933));
	SK_CSC_Gun2->SetWorldScale3D(FVector(1.0f, -1.0f, 1.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_CSC_Gun2_Asset(
		TEXT("/Game/AssetContents/CSC/Meshes/SK_CSC_Gun5.SK_CSC_Gun5"));
	if (SK_CSC_Gun2_Asset.Object)
	{
		SK_CSC_Gun2->SetSkeletalMesh(SK_CSC_Gun2_Asset.Object);
	}

	//--------------------------------------------
	SK_CSC_2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_CSC_2"));
	SK_CSC_2->SetupAttachment(SK_CSC_Body1);
	SK_CSC_2->SetRelativeLocation(FVector(1.615486, -0.935794, 107.940933));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_CSC_2_Asset(
		TEXT("/Game/AssetContents/CSC/Meshes/SK_CSC_Head2.SK_CSC_Head2"));
	if (SK_CSC_2_Asset.Object)
	{
		SK_CSC_2->SetSkeletalMesh(SK_CSC_2_Asset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UPhysicsAsset>PhysicsMeshAsset(
		TEXT("/Game/AssetContents/CSC/Meshes/CSC_Base1_PhysicsAsset_Dead.CSC_Base1_PhysicsAsset_Dead"));
	if (PhysicsMeshAsset.Object)
	{
		MeshPhsicsDeadAsset = PhysicsMeshAsset.Object;
	}
}

float AEnemyType2::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP -= DamageAmount;
	UE_LOG(LogTemp, Log, TEXT("AEnemyType2 :: TakeDamage %f"), &CurrentHP);

	if (CurrentHP <= 0)
	{
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (MeshPhsicsDeadAsset)
		{
			GetMesh()->SetPhysicsAsset(MeshPhsicsDeadAsset);
		}
		GetMesh()->SetSimulatePhysics(true);

		RemoveEnemy();
		IsDead = true;

		AEnemyAIController* AI = Cast<AEnemyAIController>(GetController());
		if (AI)
		{
			AI->IsDead(IsDead);
		}

		UE_LOG(LogTemp, Log, TEXT("AEnemyType2 :: IsDead True"));
	}

	return CurrentHP;
}

//------------------------------------------------------------------------------------
void AEnemyType2::AttackStart_Implementation()
{
	CheckAttackStart = !CheckAttackStart;
	if (CheckAttackStart)
	{
		{
			FTransform SocketT = SK_CSC_Gun1->GetSocketTransform(TEXT("Rocket"));
			FRotator Rot;
			Rot.Roll = 0;
			Rot.Pitch = 0;
			Rot.Yaw = GetActorRotation().Yaw;
			FVector Loc = SocketT.GetLocation() + UKismetMathLibrary::GetForwardVector(Rot) * 50;
			FTransform Trans = UKismetMathLibrary::MakeTransform(Loc, Rot, FVector(3.0f, 3.0f, 3.0f));

			GetWorld()->SpawnActor<AActor>(Rocket_Template, Trans);
		}
		{
			FTransform SocketT = SK_CSC_Gun2->GetSocketTransform(TEXT("Rocket"));
			FRotator Rot;
			Rot.Roll = 0;
			Rot.Pitch = 0;
			Rot.Yaw = GetActorRotation().Yaw;
			FVector Loc = SocketT.GetLocation() + UKismetMathLibrary::GetForwardVector(Rot) * 50;
			FTransform Trans = UKismetMathLibrary::MakeTransform(Loc, Rot, FVector(3.0f, 3.0f, 3.0f));

			GetWorld()->SpawnActor<AActor>(Rocket_Template, Trans);
		}
	}
}

void AEnemyType2::AttackEnd_Implementation()
{
	CheckAttackEnd = !CheckAttackEnd;
	if (CheckAttackEnd)
	{
		AEnemyAIController* AI = Cast<AEnemyAIController>(GetController());
		if (AI)
		{
			AI->IsAttackEnd(true);
			ChangeState(EEnemyState::Idle);
		}
	}
}

void AEnemyType2::RotateAttactTargetLoc_Implementation()
{
	if (AttackTarget)
	{
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
		SetActorRotation(Rot);
	}
}
