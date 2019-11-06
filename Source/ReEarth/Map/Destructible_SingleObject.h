// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Destructible_SingleObject.generated.h"

UCLASS()
class REEARTH_API ADestructible_SingleObject : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ADestructible_SingleObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* RootMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* SparkParticle;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
