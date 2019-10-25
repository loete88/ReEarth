// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class REEARTH_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USceneComponent * Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMotionControllerComponent * LeftMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent * LeftHandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent * LeftGrabSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMotionControllerComponent * RightMotionController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USkeletalMeshComponent * RightHandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent * RightGrabSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent * Camera;



	//Robot
	//------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class APlayerRobot * pRobot;

	UPROPERTY(EditAnywhere, Category = "Robot")
	TSubclassOf<class APlayerRobot> Robot_Template;
	//------------------------------------------



	//입력 Event 처리 함수
	//---------------------------------------------------
	//움직이거나 총 쏘는 것은 PlayerRobot이므로
	//아래 함수들은 전부 pRobot의 해당 함수를 호출한다.

	//이동관련 함수--------------------------------------
	//앞,뒤로 이동
	void MoveForward(float Value);

	//좌,우로 이동
	void MoveRight(float Value);

	//Robot몸체 Yaw 회전
	void ViewYaw(float Value);
	//이동관련 함수--------------------------------------


	//왼쪽 총
	void LeftShot();
	void StopLeftShot();


	//오른쪽 총
	void RightShot();
	void StopRightShot();


	//Jump
	void Jump();
	void StopJump();

	//Grab
	void Grab(bool IsLeft);
	DECLARE_DELEGATE_OneParam(GrabDeleGate, bool);
	void Release(bool IsLeft);
	DECLARE_DELEGATE_OneParam(ReleaseDeleGate, bool);

	//Vr없을 때 테스트할 수 있도록 만든 함수
	void LockOff();
	//---------------------------------------------------
	//입력 Event 처리 함수




	//VRHand 관련 함수
	//----------------------------------------------------


	//GetActornearHand함수
	//----------------------------------------------------
	//가장 가까이 있고 집는 것이 가능한 Actor를 반환해준다.
	UFUNCTION()
	class ACanDropActor * GetActorNearHand(bool bIsLeft);
	//----------------------------------------------------

};
