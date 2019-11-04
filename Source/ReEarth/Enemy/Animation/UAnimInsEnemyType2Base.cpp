// Fill out your copyright notice in the Description page of Project Settings.


#include "UAnimInsEnemyType2Base.h"
#include "Kismet/KismetMathLibrary.h"

void UUAnimInsEnemyType2Base::NativeUpdateAnimation(float DeltaSeconds)
{
	Pawn = Cast<AEnemyBase>(TryGetPawnOwner());
	if (Pawn)
	{
		State = Pawn->State;

		Pawn->GetVelocity();
		UKismetMathLibrary::VSizeXYSquared(Pawn->GetVelocity()) != 0.0f ? IsRunning = true : IsRunning = false;
	}
}

void UUAnimInsEnemyType2Base::AnimNotify_DeadEnd()
{
	if (Pawn)
	{
		Pawn->DeadEnd();
	}
}
