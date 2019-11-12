// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUIBase.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UMainUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	RobotHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Bar")));

	HomingCoolTimeBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Cycle")));

	for (int iCnt = 0; iCnt < df_MISSILE_NUM; ++iCnt)
	{
		FString MissileText = FString("Missile_").Append(FString::FromInt(iCnt+1));
		Missile.Add(Cast<UImage>(GetWidgetFromName((FName)(*MissileText))));
	}


	//초기에는 꽉 차있는 상태
	SetHPBar();

	SetHomingCoolTimeBar();
}

void UMainUIBase::SetHPBar(float CurrentHP_Rate)
{
	RobotHPBar->SetPercent(CurrentHP_Rate);
}

void UMainUIBase::SetHomingCoolTimeBar(float CurrentCoolTime_Rate)
{
	HomingCoolTimeBar->SetPercent(CurrentCoolTime_Rate);
}

void UMainUIBase::UpdateRemoveMissile()
{
	--iMissileUINum;
	SetMissileState(ESlateVisibility::Hidden, iMissileUINum);
}

void UMainUIBase::UpdateAddMissile()
{
	SetMissileState(ESlateVisibility::Visible, iMissileUINum);
	++iMissileUINum;
}

void UMainUIBase::UpdateAllMissile()
{
	for (int iCnt = 0; iCnt < df_MISSILE_NUM; ++iCnt)
	{
		Missile[iCnt]->SetVisibility(ESlateVisibility::Visible);
	}
	iMissileUINum = 4;
}

void UMainUIBase::SetMissileState(ESlateVisibility State, int iIdx)
{
	Missile[iIdx]->SetVisibility(State);
}

