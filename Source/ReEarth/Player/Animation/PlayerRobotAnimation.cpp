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
		//Velocity ����
		//-----------------------------------------------------------------------------------
		Velocity = UKismetMathLibrary::VSizeXY(PlayerRobot->GetCharacterMovement()->Velocity);
		//-----------------------------------------------------------------------------------


		//�޼� Aim Pitch, Yaw�� ����
		//-----------------------------------------------------------------------------------

		APlayerPawn * pPlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		//1. ���� ���ɿ��� �Ǵ�
		if (PlayerRobot->LeftAttackState != EHandState::Off)
		{
			//2. PlayerPawn�� �޼� ��������
			//-----------------------------------------------

			if (pPlayerPawn)
			{
				USkeletalMeshComponent * LeftHandMesh = pPlayerPawn->LeftHandMesh;

				//3. Pitch, Yaw�� Setting
				FRotator HandRotator = LeftHandMesh->K2_GetComponentRotation();
				LeftAimYaw = HandRotator.Yaw;
				LeftAimPitch = HandRotator.Pitch;
				//UE_LOG(LogClass, Warning,TEXT("%f"), LeftAimYaw);
			}
			//-----------------------------------------------
		}
		//-----------------------------------------------------------------------------------


		//������ Aim Pitch, Yaw�� ����
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


		//IsJumping �� ����
		IsJumping = PlayerRobot->GetCharacterMovement()->IsFalling();

	}
}
