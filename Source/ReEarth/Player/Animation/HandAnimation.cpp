// Fill out your copyright notice in the Description page of Project Settings.


#include "HandAnimation.h"
#include "Components/SkeletalMeshComponent.h"

void UHandAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	USkeletalMeshComponent * pMesh =  GetOwningComponent();
	APlayerPawn * pPlayerPawn = Cast<APlayerPawn>(TryGetPawnOwner());
	
	if (pPlayerPawn && pMesh)
	{
		if (pPlayerPawn->LeftHandMesh == pMesh)
		{
			VRHandState = pPlayerPawn->LeftVRHandState;
		}
		else
		{
			VRHandState = pPlayerPawn->RightVRHandState;
		}
	}
}
