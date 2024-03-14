#include "Characters/ClimbComponent.h"
#include "Character/ALSCharacter.h"
#include "Character/ALSCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Interfaces/ClimbInterface.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"



UClimbComponent::UClimbComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}
void UClimbComponent::SetClimbState(EClimbState NewState)
{
	if (ClimbState != NewState)
	{
		ClimbState = NewState;
	}
}

//void UClimbComponent::ClimbValues_Implementation(EClimbState& State, bool& MovingLeft, bool& MovingRight)
//{
//	State = ClimbState;
//	MovingLeft = MovingL;
//	MovingRight = MovingR;
//}

bool UClimbComponent::ForwardTrace(FVector HeightOffset)
{
	FVector ForwardTraceValue = Player->GetActorForwardVector() * 80.f;
	const FVector Start = Player->GetActorLocation() + HeightOffset;
	const FVector End = ForwardTraceValue + Player->GetActorLocation() + HeightOffset;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(Player);

	FHitResult HitResult;

	UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		20.f,
		LedgeTraceType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		LedgeLocation = HitResult.Location;
		LedgeNormal = HitResult.ImpactNormal;
		ActorsToIgnore.AddUnique(Player);

		return true;
	}

	return false;
}

bool UClimbComponent::HeightTrace()
{
	FVector HeightTraceValue(FVector(0.f, 0.f, 100.f));
	FVector End = LedgeLocation;
	FVector Start = End + HeightTraceValue;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(Player);
	TArray<FHitResult> HitResults;
	FHitResult HitResult;

	UKismetSystemLibrary::SphereTraceMulti(
		this,
		Start,
		End,
		20.f,
		LedgeTraceType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResults,
		true
	);

	UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		20.f,
		LedgeTraceType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		LedgeHeight = HitResults.Last().ImpactPoint;
	}
	else
	{
		LedgeHeight = FVector::Zero();
	}

	auto Diff =	
		UKismetMathLibrary::Abs(
		UKismetMathLibrary::Abs(Player->GetMesh()->GetSocketLocation(FName("root")).Z) 
		- UKismetMathLibrary::Abs(LedgeHeight.Z)
		);

	if (Diff < GrabbableLedgeTreshold)
	{
		Player->WorldDirection = -UKismetMathLibrary::GetRightVector(UKismetMathLibrary::MakeRotFromX(LedgeNormal));
		return true;
	}

	return false;
}

void UClimbComponent::GrabLedge()
{
	if (!Grabbing)
	{
		Grabbing = true;
		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

		MovePlayerComponentTo();
	}
}

void UClimbComponent::MovePlayerComponentTo()
{
	float NormalOffset = 25.f;
	float HeightOffset = -120.f;
	FVector Location = (LedgeLocation + (LedgeNormal * NormalOffset));
	FVector Rotation = FVector(-LedgeNormal.X, Player->GetCapsuleComponent()->GetComponentRotation().Pitch, -LedgeNormal.Z);

	TEnumAsByte<EMoveComponentAction::Type> Action = EMoveComponentAction::Move;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "MovePlayerComponentFinished";
	LatentInfo.UUID = GetUniqueID();
	LatentInfo.Linkage = 0;

	UKismetSystemLibrary::MoveComponentTo(
		Player->GetCapsuleComponent(),
		FVector(Location.X, Location.Y, LedgeHeight.Z + HeightOffset),
		UKismetMathLibrary::MakeRotator(Rotation.X, Rotation.Y, Rotation.Z),
		true,
		true,
		0.1f,
		true,
		Action,
		LatentInfo
		);
}

void UClimbComponent::MovePlayerComponentFinished()
{
	Player->GetCharacterMovement()->StopMovementImmediately();

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, 0.2f, false);
	SetClimbState(EClimbState::ECS_Hanging);
	GetWorld()->GetTimerManager().SetTimer(Timer, 1.f, false);
	Grabbing = false;
}


void UClimbComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AALSCharacter>(GetOwner());
}

void UClimbComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Player->GetCharacterMovement()->IsFalling() || Player->GetMovementState() == EALSMovementState::Climb)
	{
		if (ClimbState == EClimbState::ECS_FindLedge)
		{
			if (ForwardTrace(FVector(0.f, 0.f, 70.f)))
			{
				if (HeightTrace())
				{
					Player->SetMovementState(EALSMovementState::Climb);
					ClimbState = EClimbState::ECS_GrabLedge;

					MovePlayerComponentTo();
				}
			}
		}
		else if (ClimbState == EClimbState::ECS_GrabLedge)
		{
			GrabLedge();
		}
	}

}

