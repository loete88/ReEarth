// Fill out your copyright notice in the Description page of Project Settings.


#include "StartActor.h"

// Sets default values
AStartActor::AStartActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStartActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStartActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStartActor::DoActorBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

