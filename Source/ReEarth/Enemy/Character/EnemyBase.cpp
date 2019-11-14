// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "GameFramework/PlayerController.h"
#include "Player/VRHand/PlayerPawn.h"
#include "Player/PlayerRobot.h"
#include "Components/SceneComponent.h"
#include "Enemy/Destination.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy/AI/EnemyAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Map/StageManager.h"
#include "Player/UI/Aim/HomingAimWidgetBase.h"
#include "TimerManager.h"

//------------------------------------------------------------------------------------
AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Enemy"));

	HomingPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HomingPosition"));
	HomingPosition->SetupAttachment(GetMesh());

	HomingWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HomingUI"));
	HomingWidget->SetupAttachment(RootComponent);
	HomingWidget->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	HomingWidget->SetVisibility(false);
	HomingWidget->SetDrawSize(FVector2D(700.0f, 700.0f));
	//HomingWidget->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	//AIControllerClass = AIController;
	//AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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
		//IgnoreActors.Add(this);
			
		bool IsResult = UKismetSystemLibrary::LineTraceSingle(
			GetWorld(),
			Start,
			End,
			ObstacleTraceType,
			false,
			IgnoreActors,
			EDrawDebugTrace::None,
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


	//Homing Widget이 보이는 상태일 때만 플레이어를 바라보도록
	//Aim을 회전시키자.
	if (HomingWidget->IsVisible())
	{
		//원점에 가져다 놓고
		HomingWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

		if (AttackTarget)
		{
			//플레이어를 바라봐야하는 각도 구하기
			FRotator LookRotator = UKismetMathLibrary::FindLookAtRotation(HomingWidget->GetComponentLocation(), AttackTarget->GetActorLocation());

			//Aim회전
			HomingWidget->SetWorldRotation(LookRotator);
			//앞으로 보내기
			HomingWidget->SetWorldLocation(HomingWidget->GetComponentLocation() + HomingWidget->GetForwardVector() * 400);
		}
	}

}

//------------------------------------------------------------------------------------
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	
	//미리 스폰돼있는 애들이 아닐 떄만 Enemy추가 
	

	if (!bIsSpawned)
	{
		AddEnemyToManager();
		AddEnemyToRobot();
	}
	
}

void AEnemyBase::AddEnemyToRobot()
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

void AEnemyBase::AddEnemyToManager()
{
	TArray<AActor*> arrOutActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AStageManager::StaticClass(), arrOutActors);
	for (int i = 0; i < arrOutActors.Num(); ++i)
	{
		AStageManager* SM = Cast<AStageManager>(arrOutActors[i]);
		if (SM)
		{
			UE_LOG(LogTemp, Log, TEXT("EnemyBase :: AddEnemy AStageManager")); 
			SM->AddEnemy(this);
			//SM->AddEnemy(this);
		}
	}
}

void AEnemyBase::RemoveEnemy()
{
	if (AttackTarget)
	{
		AttackTarget->pRobot->RemoveEnemy(this);
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
	if (AttackTarget)
	{
		float Dis = GetDistanceTo(AttackTarget);
		//UE_LOG(LogTemp, Log, TEXT("EnemyBase :: GetVisibleAttackTarget Dis %f"), Dis);
		if (Dis > 5000)
		{
			return false;
		}
	}
	return IsVisibleAttackTarget;
}

float AEnemyBase::GetAttackTargetRot()
{
	return 0.0f;
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Log, TEXT("EnemyBase :: TakeDamage"));
	return 0.0f;
}

//------------------------------------------------------------------------------------
void AEnemyBase::AttackStart_Implementation() { }

void AEnemyBase::AttackEnd_Implementation() { }

void AEnemyBase::DeadEnd_Implementation(){ }

//void AEnemyBase::MovePatrolLoc_Implementation() { }

//void AEnemyBase::MoveAttackTarget_Implementation() { }

//void AEnemyBase::MoveDestinationTarget_Implementation(AActor* Target) { }

void AEnemyBase::RotateAttactTargetLoc_Implementation() { }

void AEnemyBase::HomingOn()
{
	//1. Widget 가시화
	HomingWidget->SetVisibility(true);

	//2. Widget Animation Play
	UHomingAimWidgetBase * HomingAimWidget = Cast<UHomingAimWidgetBase>(HomingWidget->GetUserWidgetObject());
	HomingAimWidget->PlayWidgetAnimataion();

	GetWorldTimerManager().SetTimer(HomingOffHandle, this, &AEnemyBase::HomingOff, 1.5f,false);

}

void AEnemyBase::HomingOff()
{
	HomingWidget->SetVisibility(false);
}

void AEnemyBase::ManagerCreated()
{
	if (bIsSpawned)
		AddEnemyToManager();
}

void AEnemyBase::RobotCreated()
{
	if (bIsSpawned)
		AddEnemyToRobot();
}
