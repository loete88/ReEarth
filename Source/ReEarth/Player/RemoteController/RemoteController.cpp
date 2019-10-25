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
}

void ARemoteController::Pickup(USceneComponent * AttachTo)
{
	ACanDropActor::Pickup(AttachTo);


	AttachToComponent(AttachTo, FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld,false),TEXT("RemotePosition"));

	APlayerPawn * pPlayer = (APlayerPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	pPlayer->pRobot->RobotState = ERobotState::Normal;
	
	if (Side == EControllerSide::Left)
	{
		pPlayer->pRobot->LeftAttackState = EHandState::Attackable;
	}
	else
	{
		pPlayer->pRobot->RightAttackState = EHandState::Attackable;
	}

}

void ARemoteController::Drop()
{
	ACanDropActor::Drop();

	APlayerPawn * pPlayer = (APlayerPawn*)UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AttachToComponent(pPlayer->pRobot->Seat, FAttachmentTransformRules(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld, false), TEXT("LeftControllerPosition"));

	pPlayer->pRobot->RobotState = ERobotState::Off;

	if (Side == EControllerSide::Left)
	{
		pPlayer->pRobot->LeftAttackState = EHandState::Off;
	}
	else
	{
		pPlayer->pRobot->RightAttackState = EHandState::Off;
	}
}
