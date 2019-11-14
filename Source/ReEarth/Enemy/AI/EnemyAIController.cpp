// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"

const FName AEnemyAIController::IsAttackEndKey(TEXT("IsAttackEnd"));
const FName AEnemyAIController::IsMovePatrolLocEndKey(TEXT("IsMovePatrolLocEnd"));
const FName AEnemyAIController::IsDestinationActorEndKey(TEXT("IsDestinationActorEnd"));
const FName AEnemyAIController::IsVisibleAttackTargetKey(TEXT("IsVisibleAttackTarget"));
const FName AEnemyAIController::IsDeadKey(TEXT("IsDead"));
const FName AEnemyAIController::IsAIControllerRunKey(TEXT("IsAIControllerRun"));

AEnemyAIController::AEnemyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Enemy/AI/BB_Enemy.BB_Enemy"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	/*static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Enemy/AI/BT_Enemy.BT_Enemy"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}*/

	//if (UseBlackboard(BBAsset, Blackboard))
	//{
	//	if (RunBehaviorTree(BTAsset))
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("AEnemyAIController :: Init Success"));
	//	}
	//}
}

void AEnemyAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
}

void AEnemyAIController::IsAttackEnd(bool Value)
{
	Blackboard->SetValueAsBool(IsAttackEndKey, Value);
}

void AEnemyAIController::IsMovePatrolLocEnd(bool Value)
{
	Blackboard->SetValueAsBool(IsMovePatrolLocEndKey, Value);
}

void AEnemyAIController::IsDestinationActorEnd(bool Value)
{
	Blackboard->SetValueAsBool(IsDestinationActorEndKey, Value);
}

void AEnemyAIController::IsDead(bool Value)
{
	Blackboard->SetValueAsBool(IsDeadKey, Value);
}

void AEnemyAIController::IsAIControllerRun(bool Value)
{
	Blackboard->SetValueAsBool(IsAIControllerRunKey, Value);
}
