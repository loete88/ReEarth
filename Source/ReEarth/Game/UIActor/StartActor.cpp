// Fill out your copyright notice in the Description page of Project Settings.


#include "StartActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ReEarth_GM.h"

// Sets default values
AStartActor::AStartActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//BoxComponent 설정
	//---------------------------------------------------
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;
	Box->SetBoxExtent(FVector(50.0f, 250.0f, 50.0f));
	Box->OnComponentBeginOverlap.AddDynamic(this, &AStartActor::DoActorBeginOverlap);
	//---------------------------------------------------

	//Static Mesh 설정
	//---------------------------------------------------
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Box);
	Mesh->SetRelativeScale3D(FVector(1.0f, 5.0f, 1.0f));
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	//---------------------------------------------------

	//Widget 설정
	//---------------------------------------------------
	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(Box);
	Widget->SetCollisionProfileName(TEXT("NoCollision"));
	//---------------------------------------------------

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
	UE_LOG(LogClass, Warning, TEXT("Overlap"));
	AReEarth_GM * GM = Cast<AReEarth_GM>(UGameplayStatics::GetGameMode(GetWorld()));

	GM->GameStart();
}

