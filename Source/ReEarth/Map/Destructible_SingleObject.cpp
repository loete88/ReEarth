// Fill out your copyright notice in the Description page of Project Settings.

#include "Destructible_SingleObject.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ArrowComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"


// Sets default values
ADestructible_SingleObject::ADestructible_SingleObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootMesh"));
	RootComponent = RootMesh;
}

// Called when the game starts or when spawned
void ADestructible_SingleObject::BeginPlay()
{
	Super::BeginPlay();

	RootMesh->SetGenerateOverlapEvents(true);
	RootMesh->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
	RootMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RootMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
	RootMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
	RootMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	RootMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
	RootMesh->OnComponentBeginOverlap.AddDynamic(this, &ADestructible_SingleObject::OnOverlapBegin);
}

// Called every frame
void ADestructible_SingleObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADestructible_SingleObject::OnOverlapBegin(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel1 ||
		OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel2 ||
		OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel3 ||
		OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel4)
	{
		RootMesh->SetSimulatePhysics(true); 
		RootMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
		RootMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);

		TArray<USceneComponent*> arrow;
		RootMesh->GetChildrenComponents(false, arrow);
		for (auto var : arrow)
		{
			if (Cast<UArrowComponent>(var))
			{
				UGameplayStatics::SpawnEmitterAttached(
					SparkParticle,
					var,
					"None",
					FVector(0, 0, 0),
					FRotator(0, 0, 0),
					EAttachLocation::KeepRelativeOffset,
					true
				);
			}
		}
	}
}