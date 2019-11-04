// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy/Character/EnemyBase.h"
#include "UUAnimInsEnemyType2Gun.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API UUUAnimInsEnemyType2Gun : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AEnemyBase* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EEnemyState> State;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:

	UFUNCTION()
	void AnimNotify_AttackStart();

	UFUNCTION()
	void AnimNotify_AttackEnd();

};
