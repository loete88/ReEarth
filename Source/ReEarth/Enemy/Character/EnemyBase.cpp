// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "GameFramework/PlayerController.h"
#include "Player/VRHand/PlayerPawn.h"
#include "Components/SceneComponent.h"
#include "Enemy/Destination.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/AI/EnemyAIController.h"

AEnemyBase::AEnemyBase()
{
	UE_LOG(LogTemp, Log, TEXT("================================="));
	UE_LOG(LogTemp, Log, TEXT("AEnemyBase"));
	UE_LOG(LogTemp, Log, TEXT("================================="));

	PrimaryActorTick.bCanEverTick = true;

	HomingPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HomingPosition"));
	HomingPosition->SetupAttachment(RootComponent);

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	AttackTarget = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (AttackTarget)
	{
		//AttackTarget->pRobot->AddEnemy(this);
	}
}

void AEnemyBase::ChangeState(EEnemyState NewState)
{
	this->State = NewState;
}

bool AEnemyBase::CheckState(EEnemyState NewState)
{
	return this->State == NewState ? true : false;
}

AActor* AEnemyBase::GetDestinationTarget()
{
	TArray<AActor*> TheActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADestination::StaticClass(), TheActors);

	AActor* TmpActor = nullptr;
	for (auto & currentActor : TheActors) {
		if (currentActor->IsA(ADestination::StaticClass())) 
		{
			if (TmpActor == nullptr)
			{
				TmpActor = currentActor;
			}
			else
			{
				float Dis1 = GetDistanceTo(TmpActor);
				float Dis2 = GetDistanceTo(currentActor);

				TmpActor = Dis1 < Dis2 ? TmpActor : currentActor;
			}
		}
	}
	return TmpActor;
}

bool AEnemyBase::GetVisibleAttackTarget()
{
	bool visible = IsVisibleAttackTarget;
	IsVisibleAttackTarget = false;
	return visible;
}

void AEnemyBase::AttackStart_Implementation() { }

void AEnemyBase::AttackEnd_Implementation() { }

void AEnemyBase::DeadEnd_Implementation(){ }

void AEnemyBase::MovePatrolLoc_Implementation() { }

void AEnemyBase::MoveAttackTarget_Implementation() { }

void AEnemyBase::MoveDestinationTarget_Implementation(AActor* Target) { }

void AEnemyBase::RotateAttactTargetLoc_Implementation() { }
