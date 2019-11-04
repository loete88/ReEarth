// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyType1.h"
#include "Player/VRHand/PlayerPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Enemy/AI/EnemyAIController.h"
#include "Enemy/Weapon/RocketBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/KismetMathLibrary.h"

AEnemyType1::AEnemyType1()
{
	//--------------------------------------------
	GetCapsuleComponent()->SetCapsuleHalfHeight(500);
	GetCapsuleComponent()->SetCapsuleRadius(300);
	
	//--------------------------------------------
	GetMesh()->SetRelativeLocation(FVector(0, 0, -500));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetWorldScale3D(FVector(5.0f, 5.0f, 5.0f));
	   
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
}

//------------------------------------------------------------------------------------
void AEnemyType1::AttackStart_Implementation()
{
	CheckAttackStart = !CheckAttackStart;
	if (CheckAttackStart)
	{
		FTransform T = Weapon->GetSocketTransform(TEXT("WeaponSocket"));
		FVector Loc = T.GetLocation();

		UE_LOG(LogTemp, Log, TEXT("AttackStart_Implementation %f %f %f "), Loc.X, Loc.Y, Loc.Z);

		FRotator Rot = FRotator::ZeroRotator;
		
		//GetWorld()->SpawnActor<ARocketBase>(GetClass(), Loc, Rot);

		//FActorSpawnParameters SpawnParams;
		//SpawnParams.Owner = this;
		//SpawnParams.Instigator = Instigator;
		//GetWorld()->SpawnActor<ARocketBase>(GetClass(), Loc, Rot, SpawnParams);
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
		SetActorRotation(Rot);
	}
}
