// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRobotAnimation.h"
#include "Player/PlayerRobot.h" 
#include "Player/VRHand/PlayerPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
void UPlayerRobotAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	//Super::NativeUpdateAnimation(DeltaSeconds);


	PlayerRobot = Cast<APlayerRobot>(TryGetPawnOwner());
	if (PlayerRobot)
	{
		//Velocity 설정
		//-----------------------------------------------------------------------------------
		Velocity = UKismetMathLibrary::VSizeXY(PlayerRobot->GetCharacterMovement()->Velocity);
		//-----------------------------------------------------------------------------------


		//왼손 Aim Pitch, Yaw값 설정
		//-----------------------------------------------------------------------------------

		APlayerPawn * pPlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		//1. 동작 가능여부 판단
		if (PlayerRobot->LeftAttackState != EHandState::Off)
		{
			//2. PlayerPawn의 왼손 가져오기
			//-----------------------------------------------

			if (pPlayerPawn)
			{
				USkeletalMeshComponent * LeftHandMesh = pPlayerPawn->LeftHandMesh;

				//3. Pitch, Yaw값 Setting
				FRotator HandRotator = LeftHandMesh->K2_GetComponentRotation();
				LeftAimYaw = HandRotator.Yaw;
				LeftAimPitch = HandRotator.Pitch;
				//UE_LOG(LogClass, Warning,TEXT("%f"), LeftAimYaw);
			}
			//-----------------------------------------------
		}
		//-----------------------------------------------------------------------------------


		//오른손 Aim Pitch, Yaw값 설정
		if (PlayerRobot->RightAttackState != EHandState::Off)
		{
			if (pPlayerPawn)
			{
				USkeletalMeshComponent * RightHandMesh = pPlayerPawn->RightHandMesh;

				FRotator HandRotator = RightHandMesh->K2_GetComponentRotation();
				RightAimYaw = HandRotator.Yaw;
				RightAimPitch = HandRotator.Pitch;
			}
		}


		//IsJumping 값 설정
		IsJumping = PlayerRobot->GetCharacterMovement()->IsFalling();

	}
}
