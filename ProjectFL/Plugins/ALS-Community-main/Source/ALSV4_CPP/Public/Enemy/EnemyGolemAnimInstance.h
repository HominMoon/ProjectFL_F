#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyGolemAnimInstance.generated.h"

class AEnemyGolem;
class UCharacterMovementComponent;

UCLASS()
class ALSV4_CPP_API UEnemyGolemAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	AEnemyGolem* Golem;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsPlayerEngage_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsPlayerInBox_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsPlayerStepOn_A;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsWalking_A;


};
