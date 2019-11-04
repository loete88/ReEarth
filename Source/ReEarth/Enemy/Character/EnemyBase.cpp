// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "GameFramework/PlayerController.h"
#include "Player/VRHand/PlayerPawn.h"
#include "Player/PlayerRobot.h"
#include "Components/SceneComponent.h"
#include "Enemy/Destination.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/AI/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"

//------------------------------------------------------------------------------------
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	HomingPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HomingPosition"));
	HomingPosition->SetupAttachment(RootComponent);

	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEnemyBase::Tick(float DeltaTime)
{	
	if (AttackTarget)
	{
		FHitResult OutHit;
		
		FVector Start = GetActorLocation();
		FVector TmpEnd = AttackTarget->GetActorLocation();
		FVector End = FVector(TmpEnd.X, TmpEnd.Y, TmpEnd.Z - 300);
			
		TEnumAsByte<ETraceTypeQuery> ObstacleTraceType;

		TArray<AActor*> IgnoreActors;
		IgnoreActors.Add(this);
			
		bool IsResult = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start,
			End,
			ObstacleTraceType,
			false,
			IgnoreActors,
			EDrawDebugTrace::ForOneFrame,
			OutHit,
			true);

		if (OutHit.GetActor())
		{
			if (Cast<APlayerRobot>(OutHit.GetActor()))
			{
				IsVisibleAttackTarget = true;
			}
			else if (Cast<APlayerPawn>(OutHit.GetActor()))
			{
				IsVisibleAttackTarget = true;
			}
			else
			{
				IsVisibleAttackTarget = false;
			}
		}
		else
		{
			IsVisibleAttackTarget = false;
		}
	}
}

//------------------------------------------------------------------------------------
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEnemyBase::AddEnemy, 1.0f);
}


void AEnemyBase::AddEnemy()
{
	AttackTarget = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (AttackTarget)
	{
		UE_LOG(LogTemp, Log, TEXT("EnemyBase :: AddEnemy Success"));
		AttackTarget->pRobot->AddEnemy(this);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("EnemyBase :: AddEnemy Fail"));
	}
}

//------------------------------------------------------------------------------------
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
	return IsVisibleAttackTarget;
}

//------------------------------------------------------------------------------------
void AEnemyBase::AttackStart_Implementation() { }

void AEnemyBase::AttackEnd_Implementation() { }

void AEnemyBase::DeadEnd_Implementation(){ }

void AEnemyBase::MovePatrolLoc_Implementation() { }

void AEnemyBase::MoveAttackTarget_Implementation() { }

void AEnemyBase::MoveDestinationTarget_Implementation(AActor* Target) { }

void AEnemyBase::RotateAttactTargetLoc_Implementation() { }
