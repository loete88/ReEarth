// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerRobotAnimation.generated.h"



/**
 * 
 */
UCLASS()
class REEARTH_API UPlayerRobotAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class APlayerRobot *	PlayerRobot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float			Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float			LeftAimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float			LeftAimPitch;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float			RightAimYaw;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float			RightAimPitch;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool			IsJumping;
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


};