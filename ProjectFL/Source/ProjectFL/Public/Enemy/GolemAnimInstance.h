// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GolemAnimInstance.generated.h"

class AGolem;
class UCharacterMovementComponent;

UCLASS()
class PROJECTFL_API UGolemAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	AGolem* Golem;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsPlayerInArea_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsPlayerInRange_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsPlayerHanging_A;
};
