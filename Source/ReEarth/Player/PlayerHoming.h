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


	//로켓 Trail 효과로 사용할 Particle
	UPROPERTY(EditAnywhere, Category = "Particle")
	class UParticleSystem * TrailEffect;

	//로켓 Trail Sound로 사용할 SoundCue 
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase * TrailSound;

	//Homing On Sound로 사용할 SoundCue
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase * HomingOnSound;

	//Homing Hit로 사용할 SoundCue
	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase * HomingHitSound;

	//Target
	//-------------------------------------------------------
	//Enemy를 상속받은 Enemy Class에게로 날아간다.
	class AEnemyBase * pTarget;
	//-------------------------------------------------------

	//Shot함수
	//-------------------------------------------------------
	//APlayerRobot HomingShot함수에서 Homing준비가 되면 호출해준다.
	void Shot(AEnemyBase * Target);
	//-------------------------------------------------------

private:
	
	//Homing On함수
	//-------------------------------------------------------
	//TargetEnemy를 목표를 날아가도록 설정한다.
	void HomingOn();
	//-------------------------------------------------------

	//Homing Trail함수
	//-------------------------------------------------------
	//미사일이 날아가는 동안 Trail을 생성한다.
	void HomingTrail();
	//-------------------------------------------------------

};
