// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBullet.generated.h"

UCLASS()
class REEARTH_API APlayerBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//Component
	//----------------------------------------------
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UBoxComponent * Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent * Bullet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UProjectileMovementComponent * Movement;
	//----------------------------------------------
	//Component


	//Bullet Trail로 사용할 Particle
	UPROPERTY(EditAnywhere, Category = "Particle")
	class UParticleSystem * Trail;
	//Bullet Collision 효과롤 사용할 Particle
	UPROPERTY(EditAnywhere, Category = "Particle")
	class UParticleSystem * CollisionEffect;


	//충돌 처리
	//충돌 파티클 재생하고 상대에게 Damage를 먹인 후 파괴된다. 
	UFUNCTION()
	void DoActorBeginOverlap(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	//-----------------------------------------------------------------------
	
	
	void SendDamage(AActor * OtherActor);


private:
	const float PlayerBulletDamage = 20.0f;
};
