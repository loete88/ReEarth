// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "Player/PlayerRobot.h"
#include "Player/RemoteController/CanDropActor.h"
#include "Components/SceneComponent.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HeadMountedDisplay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PickUPActorInterface.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Player/RemoteController/RemoteController.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Scene 컴포넌트 생성
//------------------------------------------------------------
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(RootComponent);
	//------------------------------------------------------------


	//MotionController 컴포넌트 생성
	//------------------------------------------------------------
	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->SetupAttachment(Scene);

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	RightMotionController->SetupAttachment(Scene);
	//------------------------------------------------------------


	//SkeletalMesh 컴포넌트 생성
	//------------------------------------------------------------
	LeftHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	LeftHandMesh->SetupAttachment(LeftMotionController);
	LeftHandMesh->SetRelativeLocationAndRotation(FVector(-13.0f, 0.0f, -2.0f),
		FRotator(0.0f, 0.0f, 90.0f));

	RightHandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	RightHandMesh->SetupAttachment(RightMotionController);
	RightHandMesh->SetRelativeLocationAndRotation(FVector(-13.0f, 0.0f, -2.0f),
		FRotator(0.0f, 0.0f, 90.0f));
	//------------------------------------------------------------


	//SphereCollision 생성
	//------------------------------------------------------------
	LeftGrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftGrabSphere"));
	LeftGrabSphere->SetupAttachment(LeftHandMesh);
	LeftGrabSphere->SetSphereRadius(10.0f);
	LeftGrabSphere->SetRelativeLocation(FVector(14.0f, 0.0f, 2.0f));

	RightGrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightGrabSphere"));
	RightGrabSphere->SetupAttachment(RightHandMesh);
	RightGrabSphere->SetSphereRadius(10.0f);
	RightGrabSphere->SetRelativeLocation(FVector(14.0f, 0.0f, 2.0f));
	//------------------------------------------------------------

	//Camera 생성
	//------------------------------------------------------------
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(Scene);
	Camera->SetRelativeLocation(FVector(0.0f, 4.0f, 40.0f));
	//------------------------------------------------------------


	//VRHandState 설정
	//-------------------------------------------------------------
	LeftVRHandState = EVRHandState::Open;
	RightVRHandState = EVRHandState::Open;
	//-------------------------------------------------------------
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
	//--------------------------------------------------------------------------------
	//시작시 Robot 생성해서 가지고 있고 PlayerPosition에 Attach시킨다.
	pRobot = GetWorld()->SpawnActor<APlayerRobot>(Robot_Template, GetActorTransform());

	AttachToComponent(pRobot->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld,
			false),
		TEXT("PlayerPosition"));
	//--------------------------------------------------------------------------------
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerPawn::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("ViewYaw"), this, &APlayerPawn::ViewYaw);


	PlayerInputComponent->BindAction(TEXT("SetState"), IE_Pressed, this, &APlayerPawn::LockOff);

	PlayerInputComponent->BindAction(TEXT("LeftShot"), IE_Pressed, this, &APlayerPawn::LeftShot);
	PlayerInputComponent->BindAction(TEXT("LeftShot"), IE_Released, this, &APlayerPawn::StopLeftShot);

	PlayerInputComponent->BindAction(TEXT("RightShot"), IE_Pressed, this, &APlayerPawn::RightShot);
	PlayerInputComponent->BindAction(TEXT("RightShot"), IE_Released, this, &APlayerPawn::StopRightShot);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerPawn::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &APlayerPawn::StopJump);

	PlayerInputComponent->BindAction<GrabDeleGate>(TEXT("GrabLeft"), IE_Pressed, this, &APlayerPawn::Grab,true);
	PlayerInputComponent->BindAction<ReleaseDeleGate>(TEXT("GrabLeft"), IE_Released, this, &APlayerPawn::Release,true);

	PlayerInputComponent->BindAction<GrabDeleGate>(TEXT("GrabRight"), IE_Pressed, this, &APlayerPawn::Grab, false);
	PlayerInputComponent->BindAction<ReleaseDeleGate>(TEXT("GrabRight"), IE_Released, this, &APlayerPawn::Release, false);

	PlayerInputComponent->BindAction(TEXT("HomingShot"), IE_Pressed, this, &APlayerPawn::HomingShot);
}


void APlayerPawn::MoveForward(float Value)
{
	if (Value != 0)
	{
		pRobot->MoveForward(Value);
	}
}

void APlayerPawn::MoveRight(float Value)
{
	if (Value != 0)
	{
		pRobot->MoveRight(Value);
	}
}

void APlayerPawn::ViewYaw(float Value)
{
	if (Value != 0)
	{
		pRobot->ViewYaw(Value);
	}
}

void APlayerPawn::LeftShot()
{
	pRobot->LeftShot();
}

void APlayerPawn::StopLeftShot()
{
	pRobot->StopLeftShot();
}

void APlayerPawn::RightShot()
{
	pRobot->RightShot();
}

void APlayerPawn::StopRightShot()
{
	pRobot->StopRightShot();
}

void APlayerPawn::Jump()
{
	pRobot->Jump();
}

void APlayerPawn::StopJump()
{
	pRobot->StopJump();
}

void APlayerPawn::Grab(bool IsLeft)
{
	ACanDropActor * AttachedActor = nullptr;
	ARemoteController * Controller;
	if (IsLeft)
	{
		ACanDropActor * NearestActor = GetActorNearHand(IsLeft);
		if (nullptr != NearestActor)
		{
			Controller = (ARemoteController*)NearestActor;
			if (Controller->Side == EControllerSide::Right)
			{
				LeftVRHandState = EVRHandState::Grab;
				return;
			}

			AttachedActor = NearestActor;
			AttachedActor->Pickup(LeftHandMesh);
			pLeftDropActor = NearestActor;
			LeftVRHandState = EVRHandState::GrabController;
		}
		else
		{
			LeftVRHandState = EVRHandState::Grab;
		}
	}
	else
	{
		ACanDropActor * NearestActor = GetActorNearHand(IsLeft);


		if (nullptr != NearestActor)
		{
			Controller = (ARemoteController*)NearestActor;
			if (Controller->Side == EControllerSide::Left)
			{
				RightVRHandState = EVRHandState::Grab;
				return;
			}

			AttachedActor = NearestActor;
			AttachedActor->Pickup(RightHandMesh);
			pRightDropActor = NearestActor;
			RightVRHandState = EVRHandState::GrabController;
		}
		else
		{
			RightVRHandState = EVRHandState::Grab;
		}
	}
}

void APlayerPawn::Release(bool IsLeft)
{
	if (IsLeft)
	{
		if (nullptr != pLeftDropActor)
		{
			pLeftDropActor->Drop();
		}
		LeftVRHandState = EVRHandState::Open;
	}

	else
	{
		if (nullptr != pRightDropActor)
		{
			pRightDropActor->Drop();
		}
		RightVRHandState = EVRHandState::Open;
	}
}

void APlayerPawn::HomingShot()
{

}

void APlayerPawn::LockOff()
{
	pRobot->LockOff();
}

ACanDropActor* APlayerPawn::GetActorNearHand(bool bIsLeft)
{
	TArray<AActor*> OverlappedActors;
	AActor * pNearestActor = nullptr;
	float fMinDist = 1000000.0f;
	if (bIsLeft)
	{
		//1. 먼저 Sphere와 오버랩된 액터들을 배열로 가져온다.
		LeftGrabSphere->GetOverlappingActors(OverlappedActors, AActor::StaticClass());
		int iLen = OverlappedActors.Num();


		//2. 배열을 돌면서 가장 가까운 Actor를 찾는다.
		for (int iCnt = 0; iCnt < iLen; ++iCnt)
		{
			bool bImplement = UKismetSystemLibrary::DoesImplementInterface(OverlappedActors[iCnt]
				, UPickUPActorInterface::StaticClass());

			FTransform HandTransform = LeftHandMesh->GetSocketTransform(TEXT("hand_r"));
			float fCurDist = (HandTransform.GetLocation() - OverlappedActors[iCnt]->GetActorLocation()).SizeSquared2D();

			//갱신
			if (bImplement && fCurDist < fMinDist)
			{
				fMinDist = fCurDist;
				pNearestActor = OverlappedActors[iCnt];
			}
		}
	}

	else
	{
		RightGrabSphere->GetOverlappingActors(OverlappedActors, AActor::StaticClass());
		int iLen = OverlappedActors.Num();
		for (int iCnt = 0; iCnt < iLen; ++iCnt)
		{
			bool bImplement = UKismetSystemLibrary::DoesImplementInterface(OverlappedActors[iCnt]
				, UPickUPActorInterface::StaticClass());
			
			if (bImplement)
			{
				FTransform HandTransform = RightHandMesh->GetSocketTransform(TEXT("hand_r"));
				float fCurDist = (HandTransform.GetLocation() - OverlappedActors[iCnt]->GetActorLocation()).SizeSquared2D();

				//갱신
				if (fCurDist < fMinDist)
				{
					fMinDist = fCurDist;
					pNearestActor = OverlappedActors[iCnt];
				}
			}
		}
	}

	return (ACanDropActor*)pNearestActor;
}

