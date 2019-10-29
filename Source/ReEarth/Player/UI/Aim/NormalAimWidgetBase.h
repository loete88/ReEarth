// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NormalAimWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class REEARTH_API UNormalAimWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UWidgetAnimation * ShotAnimation;

	virtual void NativeConstruct() override;


	UFUNCTION(BlueprintNativeEvent)
	void TEST();
	virtual void TEST_Implementation();
};
