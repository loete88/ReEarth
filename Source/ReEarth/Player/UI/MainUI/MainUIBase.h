// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIBase.generated.h"


#define df_ALL_MISSILENUM 4

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UImage*> Missile;

	
	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetHPBar(float CurrentHP_Rate = 1.0f);

	//미사일 1개 숨겨주는 함수
	void UpdateRemoveMissile();

	//미사일 1개 보여주는 함수
	void UpdateAddMissile();

private:
	int CurrentMissileMaxIdx = 3;


	void SetMissileVisibility(ESlateVisibility NewState, int iIdx);
};
