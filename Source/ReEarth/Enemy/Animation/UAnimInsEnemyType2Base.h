// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy/Character/EnemyBase.h"
#include "UAnimInsEnemyType2Base.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API UUAnimInsEnemyType2Base : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AEnemyBase* Pawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EEnemyState> State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsRunning;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:

	UFUNCTION()
	void AnimNotify_DeadEnd();

};
