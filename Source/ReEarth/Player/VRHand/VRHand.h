// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/RemoteController/CanDropActor.h"
#include "VRHand.generated.h"

//EHandState ENum
//-------------------------------------------------------------------
//Open : Ʈ���Ÿ� ������ ���� ����
//GrabController : Ʈ���Ÿ� ������ Controller�� ����ִ� ����
//Grab : Ʈ���Ÿ� �����µ� ������ Controller�� ���� ����
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
//���� Hand�� �޼����� ���������� ���������� ENum��
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

	
	//Grab���� ��� ����
	//------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Grab")
	class ACanDropActor * AttachedActor;//�տ� ���� �� Actor

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab")
	EVRHandState HandState = EVRHandState::Open;//���� �� ����

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab")
	bool bWantsToGrab = false;
	//------------------------------------------------------------

	//Grab���� ��� �Լ�
	//------------------------------------------------------------
	//���� �� ��ġ���� ����� Actor�� ��ȯ�Ѵ�.
	//����� Actor�� ���� ��� nullptr��ȯ
	//UFUNCTION()
	//ACanDropActor * GetActorNearHand();

	UFUNCTION()
	void GrabActor();

	UFUNCTION()
	void ReleaseActor();

	//------------------------------------------------------------

	//Init���� ��� ����
	//------------------------------------------------------------
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Init")
	EHandSide HandSide;
	//------------------------------------------------------------


	UFUNCTION(BlueprintImplementableEvent, Category = "RoomScale")
	void SetupRoomScaleOutline();

	UFUNCTION(BlueprintImplementableEvent, Category = "RoomScale")
	void UpdateRoomScaleOutline();




};