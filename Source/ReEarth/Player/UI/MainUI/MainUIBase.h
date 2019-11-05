// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIBase.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API UMainUIBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UProgressBar * RobotHPBar;
	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetHPBar(float CurrentHP_Rate = 1.0f);
};
