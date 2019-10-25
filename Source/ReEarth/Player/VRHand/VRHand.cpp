// Fill out your copyright notice in the Description page of Project Settings.


#include "VRHand.h"
#include "Components/SceneComponent.h"
#include "MotionControllerComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HeadMountedDisplay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PickUPActorInterface.h"

// Sets default values
AVRHand::AVRHand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Scene ������Ʈ ����
	//------------------------------------------------------------
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(RootComponent);
	//------------------------------------------------------------


	//MotionController ������Ʈ ����
	//------------------------------------------------------------
	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionController"));
	MotionController->SetupAttachment(Scene);
	//------------------------------------------------------------


	//SkeletalMesh ������Ʈ ����
	//------------------------------------------------------------
	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(MotionController);
	HandMesh->SetRelativeLocationAndRotation(FVector(-13.0f, 0.0f, -2.0f),
		FRotator(0.0f, 0.0f, 90.0f));
	//------------------------------------------------------------


	//SphereCollision ����
	//------------------------------------------------------------
	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphere"));
	GrabSphere->SetupAttachment(HandMesh);
	GrabSphere->SetSphereRadius(10.0f);
	GrabSphere->SetRelativeLocation(FVector(14.0f, 0.0f, 2.0f));
	//------------------------------------------------------------


	//StaticMesh ����
	//------------------------------------------------------------
	RoomScaleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RoomScaleMesh"));
	RoomScaleMesh->SetupAttachment(MotionController);
	//------------------------------------------------------------


	if (HandSide == EHandSide::Left)
		MotionController->SetTrackingMotionSource("Left");
	else
		MotionController->SetTrackingMotionSource("Right");

}


// Called when the game starts or when spawned
void AVRHand::BeginPlay()
{
	Super::BeginPlay();
	
	SetupRoomScaleOutline();
}

// Called every frame
void AVRHand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//ACanDropActor * AVRHand::GetActorNearHand()
//{
//	//1. ���� Sphere�� �������� ���͵��� �迭�� �����´�.
//	TArray<AActor*> OverlappedActors;
//	GrabSphere->GetOverlappingActors(OverlappedActors,AActor::StaticClass());
//	int iLen = OverlappedActors.Num();
//
//
//	//2. �迭�� ���鼭 ���� ����� Actor�� ã�´�.
//	AActor * pNearestActor = nullptr;
//	float fMinDist = 1000000.0f;
//
//	for (int iCnt = 0; iCnt < iLen; ++iCnt)
//	{
//		bool bImplement = UKismetSystemLibrary::DoesImplementInterface(OverlappedActors[iCnt]
//			, UPickUPActorInterface::StaticClass());
//
//		float fCurDist = (GetActorLocation() - pNearestActor->GetActorLocation()).SizeSquared2D();
//
//		//����
//		if (bImplement && fCurDist < fMinDist)
//		{
//			fMinDist = fCurDist;
//			pNearestActor = OverlappedActors[iCnt];
//		}
//	}
//
//
//	return (ACanDropActor*)pNearestActor;
//}

void AVRHand::GrabActor()
{
	bWantsToGrab = true;

	//ACanDropActor * NearestActor = GetActorNearHand();

	/*if (nullptr != NearestActor)
	{
		AttachedActor = NearestActor;
		AttachedActor->Pickup(HandMesh);
	}
*/
}

void AVRHand::ReleaseActor()
{
	bWantsToGrab = false;

	if (nullptr != AttachedActor)
	{
		USceneComponent * Scene = AttachedActor->K2_GetRootComponent()->GetAttachParent();
		if (Scene == HandMesh)
		{
			AttachedActor->Drop();
			AttachedActor = nullptr;
		}
	}
}

