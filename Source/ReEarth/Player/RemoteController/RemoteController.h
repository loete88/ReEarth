// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/RemoteController/CanDropActor.h"
#include "RemoteController.generated.h"


//------------------------------------
//���� Controller, ������ Controller��
//�з��ϱ� ���� ENUM��
UENUM(BlueprintType)
enum class EControllerSide : uint8
{
	Left = 0		UMETA(DisplayName = "Left"),
	Right = 1		UMETA(DisplayName = "Right")
};
//------------------------------------

/**
 * 
 */
UCLASS()
class REEARTH_API ARemoteController : public ACanDropActor
{
	GENERATED_BODY()


public:
	ARemoteController();

	virtual void Pickup(USceneComponent * AttachTo) override;
	virtual void Drop() override;


	//Component
	//---------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USphereComponent * Sphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent * Controller;
	//---------------------------------------------
	//Component


	UPROPERTY(EditAnywhere, Category = "Side")
	EControllerSide Side;
};
