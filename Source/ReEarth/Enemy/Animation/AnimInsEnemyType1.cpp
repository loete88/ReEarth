// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInsEnemyType1.h"
#include "Kismet/KismetMathLibrary.h"

void UAnimInsEnemyType1::NativeUpdateAnimation(float DeltaSeconds)
{
	Pawn = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Pawn)
	{
		State = Pawn->State;

		Pawn->GetVelocity();
		UKismetMathLibrary::VSizeXYSquared(Pawn->GetVelocity()) != 0.0f ? IsRunning = true : IsRunning = false;

		AttackTargetRot = Pawn->GetAttackTargetRot();
	}
}

void UAnimInsEnemyType1::AnimNotify_AttackStart()
{
	if (Pawn)
	{
		Pawn->AttackStart();
	}
	
}

void UAnimInsEnemyType1::AnimNotify_AttackEnd()
{
	if (Pawn)
	{
		Pawn->AttackEnd();
	}
}

void UAnimInsEnemyType1::AnimNotify_DeadEnd()
{
	if (Pawn)
	{
		Pawn->DeadEnd();
	}
}
