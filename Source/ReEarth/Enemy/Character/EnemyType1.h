// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Character/EnemyBase.h"
#include "EnemyType1.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API AEnemyType1 : public AEnemyBase
{
	GENERATED_BODY()
	
public:

	AEnemyType1();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CheckAttackStart = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CheckAttackEnd = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> Rocket_Template;

	float GetAttackTargetRot();

	//------------------------------------------------------------------------------------
	void AttackStart_Implementation();

	void AttackEnd_Implementation();

	void RotateAttactTargetLoc_Implementation();

	//------------------------------------------------------------------------------------
};
