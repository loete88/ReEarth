// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalAim.h"
#include "Components/WidgetComponent.h"
#include "Player/UI/Aim/NormalAimWidgetBase.h"

// Sets default values
ANormalAim::ANormalAim()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Aim = CreateDefaultSubobject<UWidgetComponent>(TEXT("Aim"));
	Aim->SetupAttachment(Scene);
	Aim->SetDrawSize(FVector2D(200.0f, 200.0f));

}

// Called when the game starts or when spawned
void ANormalAim::BeginPlay()
{
	Super::BeginPlay();


	AimWidget = Cast<UNormalAimWidgetBase>(Aim->GetUserWidgetObject());
}

// Called every frame
void ANormalAim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ANormalAim::PlayAnimation()
{
	if(AimWidget)
		AimWidget->TEST();
}