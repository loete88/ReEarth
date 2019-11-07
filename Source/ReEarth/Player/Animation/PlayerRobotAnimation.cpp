// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRobotAnimation.h"
#include "Player/VRHand/PlayerPawn.h"
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
				LeftAimYaw = ComputeYaw(LeftHandMesh);
				LeftAimPitch = HandRotator.Pitch;
				//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%f"), LeftAimYaw));
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
				RightAimYaw = ComputeYaw(RightHandMesh);
				RightAimPitch = HandRotator.Pitch;
			}
		}


		//IsJumping �� ����
		IsJumping = PlayerRobot->GetCharacterMovement()->IsFalling();

	}
}

float UPlayerRobotAnimation::ComputeYaw(USkeletalMeshComponent * Hand)
{
	APlayerPawn * PlayerPawn = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (!PlayerPawn->pRobot)
	{
		return 0;
	}
	//�� Forward Vector ��������
	FVector HandForward = Hand->GetForwardVector();

	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Hand:%s"), *Hand->GetComponentRotation().ToString()));


	//�� Forward Vector Z�� ����
	FVector HandForwardXY = UKismetMathLibrary::MakeVector(HandForward.X, HandForward.Y, 0.0f);

	//�κ� Forward Vector ��������
	FVector RobotForward = PlayerPawn->pRobot->GetActorForwardVector();

	//�κ� Forward Vector Z�� ����
	FVector RobotForwardXY = UKismetMathLibrary::MakeVector(RobotForward.X, RobotForward.Y, 0.0f);

	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Robot:%s"), *PlayerPawn->pRobot->GetActorRotation().ToString()));

	//�κ��� �� Dot Product
	float ResultDotProduct = UKismetMathLibrary::Dot_VectorVector(HandForwardXY, RobotForwardXY);

	float HandForwardLength = UKismetMathLibrary::VSize(HandForwardXY);
	float RobotForwardLength = UKismetMathLibrary::VSize(RobotForwardXY);

	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Len:%f"), HandForwardLength * RobotForwardLength));


	//Dot Product�� �̿��ؼ� ������� Hand Angle ���ϱ�
	float Angle = UKismetMathLibrary::DegAcos(ResultDotProduct / (HandForwardLength*RobotForwardLength));
	

	//���� ��Į�� �� Angle���� ���� ���� �� 
	//���� ���� �� ��ȣ�� ���ؾ���

	FVector RobotRightVector = PlayerPawn->pRobot->GetActorRightVector();
	FVector RobotRightVectorXY = UKismetMathLibrary::MakeVector(RobotRightVector.X, RobotRightVector.Y, 0.0f);

	ResultDotProduct = UKismetMathLibrary::Dot_VectorVector(RobotRightVectorXY, HandForwardXY);

	if (ResultDotProduct < 0)
	{
		Angle *= -1;
	}

	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Angle:%f"), Angle));

	return Angle;
}
