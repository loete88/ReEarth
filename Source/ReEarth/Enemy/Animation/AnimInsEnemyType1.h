// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy/Character/EnemyBase.h"
#include "AnimInsEnemyType1.generated.h"

UCLASS()
class REEARTH_API UAnimInsEnemyType1 : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AEnemyBase* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState State;
	//TEnumAsByte<EEnemyState> State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackTargetRot;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
private:
	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_DeadEnd();

};
