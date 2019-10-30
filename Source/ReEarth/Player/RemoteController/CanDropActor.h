// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/VRHand/PickUPActorInterface.h"
#include "CanDropActor.generated.h"

UCLASS()
class REEARTH_API ACanDropActor : public AActor  ,public IPickUPActorInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACanDropActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Pickup(USceneComponent * AttachTo);
	virtual void Drop();
};
