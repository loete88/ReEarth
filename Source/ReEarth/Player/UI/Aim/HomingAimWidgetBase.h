// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HomingAimWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API UHomingAimWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetAnimation * ShotAnimation;

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent)
	void PlayWidgetAnimataion();
	void PlayWidgetAnimataion_Implementation();
};
