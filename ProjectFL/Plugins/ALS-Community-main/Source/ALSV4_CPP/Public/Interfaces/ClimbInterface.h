// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ClimbState.h"
#include "ClimbInterface.generated.h"

UINTERFACE(MinimalAPI)
class UClimbInterface : public UInterface
{
	GENERATED_BODY()
};

class ALSV4_CPP_API IClimbInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void ClimbValues(EClimbState& OutState, bool& OutMovingLeft, bool& OutMovingRight, bool& OutFullHang);

};
