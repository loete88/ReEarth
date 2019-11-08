// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"


//Hand Grab���¿� ���� ENum��
UENUM(BlueprintType)
enum class EVRHandState : uint8
{
	Open			= 0		UMETA(DisplayName = "Open"),
	GrabController	= 1		UMETA(DisplayName = "GrabController"),
	Grab			= 2		UMETA(DisplayName = "Grab")
};


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


	//VRHandState
	//------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EVRHandState LeftVRHandState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EVRHandState RightVRHandState;
	//------------------------------------------


	//�Է� Event ó�� �Լ�
	//---------------------------------------------------
	//�����̰ų� �� ��� ���� PlayerRobot�̹Ƿ�
	//�Ʒ� �Լ����� ���� pRobot�� �ش� �Լ��� ȣ���Ѵ�.

	//�̵����� �Լ�--------------------------------------
	//��,�ڷ� �̵�
	//UFUNCTION(Server, Reliable, WithValidation)
	void MoveForward(float Value);
	//bool MoveForward_Validate(float Value);
	//void MoveForward_Implementation(float Value);


	//��,��� �̵�
	void MoveRight(float Value);

	//Robot��ü Yaw ȸ��
	void ViewYaw(float Value);
	//�̵����� �Լ�--------------------------------------


	//���� ��
	void LeftShot();
	void StopLeftShot();


	//������ ��
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


	//Homing
	void HomingShot();

	//VRReset
	void VRReset();

	//Vr���� �� �׽�Ʈ�� �� �ֵ��� ���� �Լ�
	void LockOff();
	//---------------------------------------------------
	//�Է� Event ó�� �Լ�



	//GetActornearHand�Լ�
	//----------------------------------------------------
	//���� ������ �ְ� ���� ���� ������ Actor�� ��ȯ���ش�.
	UFUNCTION()
	class ACanDropActor * GetActorNearHand(bool bIsLeft);
	//----------------------------------------------------



private:
	//�� �տ� ����ִ� Actor�� ����
	//----------------------------------------------------
	class ACanDropActor * pLeftDropActor;
	class ACanDropActor * pRightDropActor;
	//----------------------------------------------------
};
