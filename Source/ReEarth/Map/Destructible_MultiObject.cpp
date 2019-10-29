// Fill out your copyright notice in the Description page of Project Settings.


#include "Destructible_MultiObject.h"
#include "Components\BoxComponent.h"
#include "Components\ArrowComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Classes\Components\ShapeComponent.h"

// Sets default values
ADestructible_MultiObject::ADestructible_MultiObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	Box->SetupAttachment(RootComponent);
	Box->SetGenerateOverlapEvents(false);
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ADestructible_MultiObject::BeginPlay()
{
	Super::BeginPlay();

	FindChildComponent(Box, ChildInChildComponent);
	for (auto var : ChildInChildComponent)
	{
		UBoxComponent* box = Cast<UBoxComponent>(var);
		if (box != nullptr)
		{
			box->AreaClass = AreaClass;
			box->GetBodyInstance()->bOverrideMass = true;
			box->SetMassOverrideInKg(NAME_None, 10);
			box->GetBodyInstance()->UpdateMassProperties();
			box->SetPhysMaterialOverride(PM);
		}
	}
}

// Called every frame
void ADestructible_MultiObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestructible_MultiObject::OnCompHit(UPrimitiveComponent * HitComp, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if (OtherComp->GetCollisionObjectType() == ECollisionChannel::ECC_GameTraceChannel2 && !OtherComp->IsPhysicsCollisionEnabled())
	{
		HitComp->SetSimulatePhysics(true);
		ChildInChildComponent.Empty();
		FindChildComponent(Cast<UBoxComponent>(HitComp), ChildInChildComponent);
		ChildInChildComponent.Add(Cast<UBoxComponent>(HitComp));
		for (auto var : ChildInChildComponent)
		{
			UPrimitiveComponent* SimpleCast = Cast<UPrimitiveComponent>(var);
			if (SimpleCast != nullptr)
			{
				var->SetSimulatePhysics(true);
				var->AddRadialImpulse(OtherComp->GetComponentLocation(), 50000, 30000, RIF_Linear, false);
				var->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Ignore);
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

void ADestructible_MultiObject::FindChildComponent(UBoxComponent* Target, TArray<UBoxComponent*> SetIndex)
{
	TArray<USceneComponent*> CurrentChild;
	int Length = CurrentChild.Max();
	if (Length > 0)
	{
		for(auto var : CurrentChild)
		{
			UBoxComponent* Box = Cast<UBoxComponent>(var);
			if (Box != nullptr)
			{
				SetIndex.Add(Box);
				FindChildComponent(Box, SetIndex);
			}
		}
	}
}
