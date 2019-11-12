// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Character/EnemyBase.h"
#include "EnemyType2.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API AEnemyType2 : public AEnemyBase
{
	GENERATED_BODY()

public:

	AEnemyType2();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SK_CSC_Body1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SK_CSC_1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SK_CSC_Gun1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SK_CSC_Gun2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SK_CSC_2;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CheckAttackStart = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool CheckAttackEnd = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> Rocket_Template;

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//------------------------------------------------------------------------------------
	void AttackStart_Implementation();

	void AttackEnd_Implementation();

	void RotateAttactTargetLoc_Implementation();

	//------------------------------------------------------------------------------------
	
};
