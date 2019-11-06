// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructible_MultiObject.h"
#include "Components\BoxComponent.h"
#include "Components\StaticMeshComponent.h"
#include "Components\ArrowComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Classes\Components\ShapeComponent.h"

// Sets default values
ADestructible_MultiObject::ADestructible_MultiObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxRoot = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxRoot"));
	BoxRoot->SetupAttachment(RootComponent);
	BoxRoot->SetGenerateOverlapEvents(false);
	BoxRoot->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ADestructible_MultiObject::BeginPlay()
{
	Super::BeginPlay();

	FindChildComponent(BoxRoot, ChildInChildComponent);
	for (auto var : ChildInChildComponent)
	{
		var->AreaClass = AreaClass;
		var->GetBodyInstance()->bOverrideMass = true;
		var->SetMassOverrideInKg(NAME_None, 10);
		var->GetBodyInstance()->UpdateMassProperties();
		var->SetPhysMaterialOverride(PM);
		var->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		var->SetCollisionObjectType(ECollisionChannel::ECC_Destructible);
		var->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		var->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
		var->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Overlap);
		var->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
		var->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
		var->OnComponentBeginOverlap.AddDynamic(this, &ADestructible_MultiObject::OnOverlapBegin);
	}
}

// Called every frame
void ADestructible_MultiObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestructible_MultiObject::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp,
	class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel1 ||
		OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel2 ||
		OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel3 ||
		OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel4)
	{
		OverlappedComp->SetSimulatePhysics(true);
		ChildInChildComponent.Empty();
		FindChildComponent(Cast<UBoxComponent>(OverlappedComp), ChildInChildComponent);
		ChildInChildComponent.Add(Cast<UBoxComponent>(OverlappedComp));
		for (auto var : ChildInChildComponent)
		{
			UPrimitiveComponent* SimpleCast = Cast<UPrimitiveComponent>(var);
			if (SimpleCast != nullptr)
			{
				var->SetSimulatePhysics(true);
				var->AddRadialImpulse(OtherComp->GetComponentLocation(), ImpulseRadius, ImpulseStrength, RIF_Linear, false);
				var->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
				var->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel3, ECollisionResponse::ECR_Ignore);
				TArray<USceneComponent*> child;
				var->GetChildrenComponents(false, child);
				for (auto childVar : child)
				{
					UArrowComponent* arrow = Cast<UArrowComponent>(childVar);
					if (arrow != nullptr)
					{
						UGameplayStatics::SpawnEmitterAttached(
							SparkParticle,
							childVar,
							"None",
							FVector(0, 0, 0),
							FRotator(0, 0, 0),
							EAttachLocation::KeepRelativeOffset,
							true
						);

						UGameplayStatics::SpawnEmitterAttached(
							ExplosionParticle,
							childVar,
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
	}
}

void ADestructible_MultiObject::FindChildComponent(UBoxComponent* Target, TArray<UBoxComponent*> &SetIndex)
{
	TArray<USceneComponent*> CurrentChild;
	Target->GetChildrenComponents(false, CurrentChild);
	int Length = CurrentChild.Max();
	if (Length > 0)
	{
		for (auto var : CurrentChild)
		{
			UBoxComponent* Box = Cast<UBoxComponent>(var);
			if (Box != nullptr)
			{
				SetIndex.Add(Box);
				FindChildComponent(Box, SetIndex);
			}
			else
			{
				UMeshComponent* Mesh = Cast<UMeshComponent>(var);
				if (Mesh != nullptr)
				{
					Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
			}
		}
	}
}
