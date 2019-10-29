// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Destructible_MultiObject.generated.h"

UCLASS()
class REEARTH_API ADestructible_MultiObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADestructible_MultiObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class UBoxComponent * Box;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UParticleSystem* SparkParticle;	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UParticleSystem* ExplosionParticle;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	class UPhysicalMaterial* PM;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<class UNavAreaBase> AreaClass;

	TArray<UBoxComponent*> ChildInChildComponent;
	bool LoopBool;
	int HP;
	int MaxHP;

	UFUNCTION()
	void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void FindChildComponent(UBoxComponent* Target, TArray<UBoxComponent*> SetIndex);
};