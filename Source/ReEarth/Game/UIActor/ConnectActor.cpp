// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ReEarth_PC.h"

// Sets default values
AConnectActor::AConnectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Box 설정
	//------------------------------------------------
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(50.0f, 250.0f, 50.0f));
	Box->OnComponentBeginOverlap.AddDynamic(this, &AConnectActor::DoActorBeginOverlap);
	//------------------------------------------------

	//Mesh 설정
	//------------------------------------------------
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetWorldScale3D(FVector(1.0f, 5.0f, 1.0f));
	Mesh->SetupAttachment(Box);
	//-----------------------------------------------

	//Widget 설정
	//-----------------------------------------------
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));
	Widget->SetupAttachment(Box);
	Widget->SetCollisionProfileName(TEXT("NoCollision"));
	//-----------------------------------------------
}

// Called when the game starts or when spawned
void AConnectActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConnectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AConnectActor::DoActorBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AReEarth_PC * PlayerController = Cast<AReEarth_PC>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PlayerController)
		PlayerController->ReqConnect();

}

