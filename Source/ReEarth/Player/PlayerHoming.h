// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerHoming.generated.h"

UCLASS()
class REEARTH_API APlayerHoming : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerHoming();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Components
	//-------------------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent * Box;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent * Homing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent * ProjectileMovement;
	//-------------------------------------------------------


	//���� Trail ȿ���� ����� Particle
	UPROPERTY(EditAnywhere, Category = "Particle")
	class UParticleSystem * TrailEffect;

	//���� Trail Sound�� ����� SoundCue 
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase * TrailSound;

	//Homing On Sound�� ����� SoundCue
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase * HomingOnSound;

	//Homing Hit�� ����� SoundCue
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase * HomingHitSound;

	//Target
	//-------------------------------------------------------
	//Enemy�� ��ӹ��� Enemy Class���Է� ���ư���.
	class AEnemyBase * pTarget;
	//-------------------------------------------------------

	//Shot�Լ�
	//-------------------------------------------------------
	//APlayerRobot HomingShot�Լ����� Homing�غ� �Ǹ� ȣ�����ش�.
	void Shot(AEnemyBase * Target);
	//-------------------------------------------------------

private:
	
	//Homing On�Լ�
	//-------------------------------------------------------
	//TargetEnemy�� ��ǥ�� ���ư����� �����Ѵ�.
	void HomingOn();
	//-------------------------------------------------------

	//Homing Trail�Լ�
	//-------------------------------------------------------
	//�̻����� ���ư��� ���� Trail�� �����Ѵ�.
	void HomingTrail();
	//-------------------------------------------------------

};
