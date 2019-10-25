// Fill out your copyright notice in the Description page of Project Settings.


#include "CanDropActor.h"

// Sets default values
ACanDropActor::ACanDropActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACanDropActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACanDropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACanDropActor::Pickup(USceneComponent * AttachTo)
{
	//���� Attach�� Actor�� Detach
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
}

void ACanDropActor::Drop()
{
	//���� Attach��(��) Actor�� Detach
	DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
}

