// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/RemoteController/CanDropActor.h"
#include "VRHand.generated.h"

//EHandState ENum
//-------------------------------------------------------------------
//Open : 트리거를 누르지 않은 상태
//GrabController : 트리거를 누르고 Controller를 쥐고있는 상태
//Grab : 트리거를 눌렀는데 주위에 Controller가 없는 상태
UENUM(BlueprintType)
enum class EVRHandState : uint8
{
	Open			= 0		UMETA(DisplayName = "Open"),
	GrabController	= 1		UMETA(DisplayName = "GrabController"),
	Grab			= 2		UMETA(DisplayName = "Grab")
};
//-------------------------------------------------------------------


//EHandSide ENum
//-------------------------------------------------------------------
//현재 Hand가 왼손인지 오른손인지 나누기위한 ENum값
UENUM(BlueprintType)
enum class EHandSide : uint8
{
	Left			= 0		UMETA(DisplayName = "Left"),
	Right			= 1		UMETA(DisplayName = "Right")
};
//-------------------------------------------------------------------


UCLASS()
class REEARTH_API AVRHand : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRHand();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	//Component
	//--------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent * Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMotionControllerComponent * MotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent * HandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent * GrabSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent * RoomScaleMesh;
	//--------------------------------------------
	//Component

	
	//Grab관련 멤버 변수
	//------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Grab")
	class ACanDropActor * AttachedActor;//손에 연결 할 Actor

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab")
	EVRHandState HandState = EVRHandState::Open;//현재 손 상태

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab")
	bool bWantsToGrab = false;
	//------------------------------------------------------------

	//Grab관련 멤버 함수
	//------------------------------------------------------------
	//현재 손 위치에서 가까운 Actor를 반환한다.
	//가까운 Actor가 없는 경우 nullptr반환
	//UFUNCTION()
	//ACanDropActor * GetActorNearHand();

	UFUNCTION()
	void GrabActor();

	UFUNCTION()
	void ReleaseActor();

	//------------------------------------------------------------

	//Init관련 멤버 변수
	//------------------------------------------------------------
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Init")
	EHandSide HandSide;
	//------------------------------------------------------------


	UFUNCTION(BlueprintImplementableEvent, Category = "RoomScale")
	void SetupRoomScaleOutline();

	UFUNCTION(BlueprintImplementableEvent, Category = "RoomScale")
	void UpdateRoomScaleOutline();




};