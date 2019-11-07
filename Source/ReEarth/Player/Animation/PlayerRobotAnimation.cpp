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
				LeftAimYaw = ComputeYaw(LeftHandMesh);
				LeftAimPitch = HandRotator.Pitch;
				//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%f"), LeftAimYaw));
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
				RightAimYaw = ComputeYaw(RightHandMesh);
				RightAimPitch = HandRotator.Pitch;
			}
		}


		//IsJumping 값 설정
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
	//손 Forward Vector 가져오기
	FVector HandForward = Hand->GetForwardVector();

	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Hand:%s"), *Hand->GetComponentRotation().ToString()));


	//손 Forward Vector Z값 제거
	FVector HandForwardXY = UKismetMathLibrary::MakeVector(HandForward.X, HandForward.Y, 0.0f);

	//로봇 Forward Vector 가져오기
	FVector RobotForward = PlayerPawn->pRobot->GetActorForwardVector();

	//로봇 Forward Vector Z값 제거
	FVector RobotForwardXY = UKismetMathLibrary::MakeVector(RobotForward.X, RobotForward.Y, 0.0f);

	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Robot:%s"), *PlayerPawn->pRobot->GetActorRotation().ToString()));

	//로봇과 손 Dot Product
	float ResultDotProduct = UKismetMathLibrary::Dot_VectorVector(HandForwardXY, RobotForwardXY);

	float HandForwardLength = UKismetMathLibrary::VSize(HandForwardXY);
	float RobotForwardLength = UKismetMathLibrary::VSize(RobotForwardXY);

	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Len:%f"), HandForwardLength * RobotForwardLength));


	//Dot Product를 이용해서 상대적인 Hand Angle 구하기
	float Angle = UKismetMathLibrary::DegAcos(ResultDotProduct / (HandForwardLength*RobotForwardLength));
	

	//위는 스칼라값 즉 Angle차이 값만 구한 것 
	//이제 방향 즉 부호를 정해야함

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
