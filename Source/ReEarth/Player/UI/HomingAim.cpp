// Fill out your copyright notice in the Description page of Project Settings.


#include "HomingAim.h"
#include "Components/WidgetComponent.h"
#include "Player/UI/Aim/HomingAimWidgetBase.h"

// Sets default values
AHomingAim::AHomingAim()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = Scene;

	Aim = CreateDefaultSubobject<UWidgetComponent>(TEXT("Aim"));
	Aim->SetupAttachment(Scene);
}

// Called when the game starts or when spawned
void AHomingAim::BeginPlay()
{
	Super::BeginPlay();
	
	AimWidget = Cast<UHomingAimWidgetBase>(Aim->GetUserWidgetObject());

	PlayAnimation();
}

// Called every frame
void AHomingAim::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHomingAim::PlayAnimation()
{
	if(AimWidget)
		AimWidget->PlayWidgetAnimataion();
}

