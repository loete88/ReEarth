// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RocketBase.generated.h"

UCLASS()
class REEARTH_API ARocketBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARocketBase();

public:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent * Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent * Rocket;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent * Movement;

	//UPROPERTY(EditAnywhere, Category = "Particle")
	//class UParticleSystem * CollisionEffect;

private:
	const float Damage = 20.0f;

};
