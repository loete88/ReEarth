// Fill out your copyright notice in the Description page of Project Settings.


#include "UUAnimInsEnemyType2Gun.h"
#include "Kismet/KismetMathLibrary.h"

void UUUAnimInsEnemyType2Gun::NativeUpdateAnimation(float DeltaSeconds)
{
	Pawn = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Pawn)
	{
		State = Pawn->State;
	}
}

void UUUAnimInsEnemyType2Gun::AnimNotify_AttackStart()
{
	if (Pawn)
	{
		Pawn->AttackStart();
	}
}

void UUUAnimInsEnemyType2Gun::AnimNotify_AttackEnd()
{
	if (Pawn)
	{
		Pawn->AttackEnd();
	}
}
