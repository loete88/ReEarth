// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIBase.h"
#include "Components/ProgressBar.h"

void UMainUIBase::NativeConstruct()
{
	RobotHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	
	//�ʱ⿡�� �� ���ִ� ����
	SetHPBar();
}

void UMainUIBase::SetHPBar(float CurrentHP_Rate)
{
	RobotHPBar->Percent = CurrentHP_Rate;
}
