// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SeekerAnimInstance.h"
#include "Characters/Seeker.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void USeekerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Seeker = Cast<ASeeker>(TryGetPawnOwner());
	if (Seeker)
	{
		MovementComponent = Seeker->GetCharacterMovement();
	}
}

void USeekerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (MovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		Velocity_A = MovementComponent->Velocity;
		IsFalling_A = MovementComponent->IsFalling();
		if (Seeker->bPressedJump)
		{
			IsJumping_A = true;
		}
	}

	if (Seeker)
	{
		if (Seeker->bIsHanging) IsHanging_A = true;
		else IsHanging_A = false;

		if (Seeker->bWasFixed == true && WasFixed_A == true) return;
		else if (Seeker->bWasFixed == false && WasFixed_A == false) return;
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("!"));
			if (Seeker->bWasFixed) WasFixed_A = true;
			else WasFixed_A = false;
		}
		

		if (Seeker->bIsClimbingLedge) IsClimbing_A = true;
		else IsClimbing_A = false;
	}
}
