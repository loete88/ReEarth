// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainUIBase.generated.h"

#define df_MISSILE_NUM 4

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


	//UI를 보여줄지 숨길지 Set해주는 함수------------------
	UFUNCTION()
	void SetMissileState(ESlateVisibility State, int iIdx);
	//-----------------------------------------------------

};
