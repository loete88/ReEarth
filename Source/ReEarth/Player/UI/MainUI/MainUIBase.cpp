// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIBase.h"
#include "Components/ProgressBar.h"

void UMainUIBase::NativeConstruct()
{
	RobotHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPBar")));
	
	//초기에는 꽉 차있는 상태
	SetHPBar();
}

void UMainUIBase::SetHPBar(float CurrentHP_Rate)
{
	RobotHPBar->Percent = CurrentHP_Rate;
}
