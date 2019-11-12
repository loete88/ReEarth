// Fill out your copyright notice in the Description page of Project Settings.


#include "StageBarrier.h"

// Sets default values
AStageBarrier::AStageBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStageBarrier::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStageBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStageBarrier::SelfDestroy()
{
	Destroy();
}