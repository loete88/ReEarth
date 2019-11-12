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
	class UProgressBar * HomingCoolTimeBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<class UImage*> Missile;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void SetHPBar(float CurrentHP_Rate = 1.0f);

	UFUNCTION()
	void SetHomingCoolTimeBar(float CurrentCoolTime_Rate = 0.0f);

	//Missile UI �Ѱ� �����
	UFUNCTION()
	void UpdateRemoveMissile();

	//Missile UI �Ѱ� �߰�
	UFUNCTION()
	void UpdateAddMissile();

	UFUNCTION()
	void UpdateAllMissile();


private:
	//UI�� �������� ������ Set���ִ� �Լ�------------------
	void SetMissileState(ESlateVisibility State, int iIdx);
	//-----------------------------------------------------

	//���� UI�� �̻��� ����
	int iMissileUINum = 4;
	//----------------------

};
