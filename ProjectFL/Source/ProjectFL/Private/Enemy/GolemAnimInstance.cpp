#include "Enemy/GolemAnimInstance.h"
#include "Enemy/Golem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UGolemAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UGolemAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);


	if (Golem)
	{
		if (Golem->bIsPlayerInArea) IsPlayerInArea_A = true;
		else IsPlayerInArea_A = false;

		if (Golem->bIsPlayerInArea) IsPlayerInRange_A = true;
		else IsPlayerInRange_A = false;

		if (Golem->bIsPlayerInArea) IsPlayerHanging_A = true;
		else IsPlayerHanging_A = false;
	}
}