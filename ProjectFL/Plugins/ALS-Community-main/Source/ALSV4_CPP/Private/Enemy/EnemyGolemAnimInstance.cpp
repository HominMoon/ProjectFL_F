#include "Enemy/EnemyGolemAnimInstance.h"
#include "Enemy/EnemyGolem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEnemyGolemAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Golem = Cast<AEnemyGolem>(TryGetPawnOwner());
	if (Golem)
	{
		MovementComponent = Cast<UCharacterMovementComponent>(Golem->GetCharacterMovement());
	}


}

void UEnemyGolemAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Golem)
	{
		if (Golem->IsPlayerEngage) IsPlayerEngage_A = true;
		else IsPlayerEngage_A = false;

		if (Golem->IsPlayerInBox) IsPlayerInBox_A = true;
		else IsPlayerInBox_A = false;

		if (Golem->IsPlayerStepOn) IsPlayerStepOn_A = true;
		else IsPlayerStepOn_A = false;

		if (Golem->IsWalking) IsWalking_A = true;
		IsWalking_A = false;
	}


}
