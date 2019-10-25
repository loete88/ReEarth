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



	//Target
	//-------------------------------------------------------
	//Enemy를 상속받은 Enemy Class에게로 날아간다.
	class AEnemy * pTarget;

	//-------------------------------------------------------

};
