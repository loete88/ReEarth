// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIBase.h"
#include "Components/ProgressBar.h"

void UMainUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	RobotHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Bar")));


	//�ʱ⿡�� �� ���ִ� ����
	SetHPBar();
}

void UMainUIBase::SetHPBar(float CurrentHP_Rate)
{
	RobotHPBar->SetPercent(CurrentHP_Rate);
}
