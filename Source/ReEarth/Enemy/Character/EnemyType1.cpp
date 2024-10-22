// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyType1.h"
#include "Player/VRHand/PlayerPawn.h"
#include "Player/PlayerRobot.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/AI/EnemyAIController.h"
#include "Enemy/Weapon/RocketBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/Vector.h"

AEnemyType1::AEnemyType1()
{
	MaxHP = 100.0f;
	CurrentHP = MaxHP;

	//--------------------------------------------
	GetCapsuleComponent()->SetCapsuleHalfHeight(260);
	GetCapsuleComponent()->SetCapsuleRadius(180);
	
	//--------------------------------------------
	GetMesh()->SetRelativeLocation(FVector(0, 0, -260));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetWorldScale3D(FVector(2.8f, 2.8f, 2.8f));
	   
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>MeshAsset(
		TEXT("/Game/AssetContents/ModularSciFiHeroesHP/Mesh/MeshForInternalAnimations/forIntAnim_UnifiedCharacters/Int_UnifiedCharacter01_SK.Int_UnifiedCharacter01_SK"));
	if (MeshAsset.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	//--------------------------------------------
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh());
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>WeaponAsset(
		TEXT("/Game/AssetContents/ModularSciFiHeroesHP/Mesh/MeshForInternalAnimations/forIntAnim_ModularParts/Int_AssaultRifle_SK.Int_AssaultRifle_SK"));
	if (WeaponAsset.Object)
	{
		Weapon->SetSkeletalMesh(WeaponAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UPhysicsAsset>PhysicsMeshAsset(
		TEXT("/Game/AssetContents/ModularSciFiHeroesHP/Mesh/MeshForInternalAnimations/InternalAnim_PhysicsAsset_Dead.InternalAnim_PhysicsAsset_Dead"));
	if (PhysicsMeshAsset.Object)
	{
		MeshPhsicsDeadAsset = PhysicsMeshAsset.Object;
	}
}

float AEnemyType1::GetAttackTargetRot()
{
	if (AttackTarget)
	{
		//FVector Loc1 = Weapon->GetSocketLocation(TEXT("WeaponSocket"));
		FVector Loc1 = GetActorLocation();
		FVector Loc2 = AttackTarget->pRobot->GetActorLocation();
				
		FVector EnemyForwardVector = GetActorForwardVector();
		FVector EnemyToTargetVector = Loc2 - Loc1;
		
		float ResultDotProduct = UKismetMathLibrary::Dot_VectorVector(EnemyForwardVector, EnemyToTargetVector);

		float Loc1Length = UKismetMathLibrary::VSize(EnemyForwardVector);
		float Loc2Length = UKismetMathLibrary::VSize(EnemyToTargetVector);
		float Angle = UKismetMathLibrary::DegAcos(ResultDotProduct / (Loc1Length*Loc2Length));
		
		if (Loc1.Z > Loc2.Z)
		{
			Angle *= -1;
		}

		//UE_LOG(LogTemp, Log, TEXT("UAnimInsEnemyType1 angle   :: %f"), Angle);

		return Angle;
		//return Rot.Roll;
	}
	return 0.0f;
}

float AEnemyType1::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	CurrentHP -= DamageAmount;
	UE_LOG(LogTemp, Log, TEXT("AEnemyType1 :: TakeDamage %f"), &CurrentHP);

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
		GetMesh()->AddRadialForce(GetMesh()->GetComponentLocation(), 500.0f, 99900000.0f, ERadialImpulseFalloff::RIF_Constant);
		

		RemoveEnemy();
		IsDead = true;

		AEnemyAIController* AI = Cast<AEnemyAIController>(GetController());
		if (AI)
		{
			AI->IsDead(IsDead);
		}

		UE_LOG(LogTemp, Log, TEXT("AEnemyType1 :: IsDead True"));
	}

	return CurrentHP;
}

//------------------------------------------------------------------------------------
void AEnemyType1::AttackStart_Implementation()
{
	CheckAttackStart = !CheckAttackStart;
	if (CheckAttackStart)
	{
		FTransform SocketT = Weapon->GetSocketTransform(TEXT("WeaponSocket"));
		FRotator Rot = Weapon->GetSocketRotation(TEXT("WeaponSocket"));
		FVector Loc = SocketT.GetLocation() + UKismetMathLibrary::GetForwardVector(Rot) * 50;
		FTransform Trans = UKismetMathLibrary::MakeTransform(Loc, Rot, FVector(3.0f, 3.0f, 3.0f));
		
		GetWorld()->SpawnActor<AActor>(Rocket_Template, Trans);
	}
}

void AEnemyType1::AttackEnd_Implementation()
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

void AEnemyType1::RotateAttactTargetLoc_Implementation()
{
	if (AttackTarget)
	{		
		FRotator Rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackTarget->GetActorLocation());
		FRotator ResultRot = FRotator(0, Rot.Yaw, 0);
		SetActorRotation(ResultRot);
	}
}
