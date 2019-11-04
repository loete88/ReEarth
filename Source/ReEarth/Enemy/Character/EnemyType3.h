// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Character/EnemyBase.h"
#include "EnemyType3.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API AEnemyType3 : public AEnemyBase
{
	GENERATED_BODY()

public:

	virtual bool GetVisibleAttackTarget() override;
	
};
