// Fill out your copyright notice in the Description page of Project Settings.


#include "RemoteController.h"
#include "Player/VRHand/PlayerPawn.h"
#include "Player/PlayerRobot.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"


ARemoteController::ARemoteController()
{
	//Sphere Collision 持失
	//-----------------------------------------------------------------
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(6.0f);
	//-----------------------------------------------------------------


	//Static Mesh 持失
	//-----------------------------------------------------------------
	Controller = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Controller"));
	Controller->SetupAttachment(Sphere);
	Controller->SetCollisionProfileName(TEXT("NoCollision"));
	Controller->SetRelativeLocation(FVector(1.5f, 1.5f, 3.0f));
	//-----------------------------------------------------------------

	//Static Mesh 持失
	//-----------------------------------------------------------------
	ControllerComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ControllerComponent"));
	ControllerComponent->SetupAttachment(Controller);
	ControllerComponent->SetCollisionProfileName(TEXT("NoCollision"));
	//-----------------------------------------------------------------

}

void ARemoteController::Pickup(USceneComponent * AttachTo)
{
	ACanDropActor::Pickup(AttachTo);


	APlayerPawn * pPlayer = (APlayerPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	pPlayer->pRobot->RobotState = ERobotState::Normal;
	
	if (Side == EControllerSide::Left)
	{
		AttachToComponent(AttachTo, FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("LeftRemotePosition"));
		pPlayer->pRobot->LeftAttackState = EHandState::Attackable;
		pPlayer->pRobot->RobotOnSoundPlay();
	}
	else
	{
		AttachToComponent(AttachTo, FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("RightRemotePosition"));
		pPlayer->pRobot->RightAttackState = EHandState::Attackable;
		pPlayer->pRobot->RobotOnSoundPlay();
	}

}

void ARemoteController::Drop()
{
	ACanDropActor::Drop();

	APlayerPawn * pPlayer = (APlayerPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	pPlayer->pRobot->RobotState = ERobotState::Off;

	if (Side == EControllerSide::Left)
	{
		AttachToComponent(pPlayer->pRobot->Seat, FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("LeftControllerPosition"));
		pPlayer->pRobot->LeftAttackState = EHandState::Off;
	}
	else
	{
		AttachToComponent(pPlayer->pRobot->Seat, FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld, false), TEXT("RightControllerPosition"));
		pPlayer->pRobot->RightAttackState = EHandState::Off;
	}
}
