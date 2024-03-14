#include "Components/ClimbingComponent.h"
#include "Character/ALSCharacter.h"
#include "Character/ALSCharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Character/ReferencePoint.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ClimbingComponent.h"

UClimbingComponent::UClimbingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UClimbingComponent::SetClimbState(EClimbState NewState)
{
	if (ClimbState != NewState)
	{
		ClimbState = NewState;
	}
}

void UClimbingComponent::ClimbValues_Implementation(EClimbState& OutState, bool& OutMovingLeft, bool& OutMovingRight, bool& OutFullHang)
{
	OutState = ClimbState;
	OutMovingLeft = MovingLeft;
	OutMovingRight = MovingRight;
	OutFullHang = FullHang;
}


bool UClimbingComponent::ForwardTrace(FVector HeightOffset)
{
	FullBraceTrace();

	FVector ForwardTraceValue = Player->GetActorForwardVector() * 80.f;
	const FVector Start = Player->GetActorLocation() + HeightOffset;
	const FVector End = ForwardTraceValue + Player->GetActorLocation() + HeightOffset;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(Player);

	FHitResult HitResult;

	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		Start,
		End,
		10.f,
		55.f,
		LedgeTraceType,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		WallCheck = true;
		LedgeLocation = HitResult.Location;
		LedgeNormal = HitResult.ImpactNormal;
		ActorsToIgnore.AddUnique(Player);

		return true;
	}

	WallCheck = false;
	return false;
}

bool UClimbingComponent::LedgeGrabForwardTrace(bool isLeft, UArrowComponent* Arrow)
{
	FVector ForwardTraceValue = Player->GetActorForwardVector() * 80.f;
	const float HeightOffset = 70.f;

	FVector Start = Arrow->GetComponentLocation();
	FVector End = ForwardTraceValue + Arrow->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(Player);
	if (ReferencePoint)
	{
		ActorsToIgnore.AddUnique(ReferencePoint);
	}
	

	FHitResult HitResult;

	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		Start,
		End,
		10.f,
		55.f,
		LedgeTraceType,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		if (isLeft)
		{
			LeftLedgeLocation = HitResult.Location;
			LeftLedgeNormal = HitResult.ImpactNormal;
		}
		else
		{
			RightLedgeLocation = HitResult.Location;
			RightLedgeNormal = HitResult.ImpactNormal;
		}
		return true;
	}

	return false;
}

bool UClimbingComponent::HeightTrace()
{
	FVector HeightTraceValue(FVector(0.f, 0.f, 100.f));
	FVector End = LedgeLocation;
	FVector Start = End + HeightTraceValue;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(Player);
	TArray<FHitResult> HitResults;
	FHitResult HitResult;



	UKismetSystemLibrary::CapsuleTraceMulti(
		this,
		Start,
		End,
		20.f,
		10.f,
		LedgeTraceType,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResults,
		true
	);

	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		Start,
		End,
		20.f,
		10.f,
		LedgeTraceType,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		FResult = HitResults.Last();
		LedgeHeight = HitResults.Last().ImpactPoint;
	}
	else
	{
		LedgeHeight = FVector::Zero();
		HeightCheck = false;
		return false;
	}

	auto Diff =
		UKismetMathLibrary::Abs(
			UKismetMathLibrary::Abs(Player->GetMesh()->GetSocketLocation(FName("root")).Z)
			- UKismetMathLibrary::Abs(LedgeHeight.Z)
		);

	if (Diff < GrabbableLedgeTreshold)
	{
		Player->WorldDirection = -UKismetMathLibrary::GetRightVector(UKismetMathLibrary::MakeRotFromX(LedgeNormal));
		HeightCheck = true;
		return true;
	}

	HeightCheck = false;
	return false;
}

bool UClimbingComponent::LedgeGrabHeightTrace(bool isLeft, UArrowComponent* Arrow)
{
	FVector HeightTraceValue(FVector(0.f, 0.f, 100.f));
	FVector End;
	FVector Start;

	if (isLeft)
	{
		End = LeftLedgeLocation;
	}
	else
	{
		End = RightLedgeLocation;
	}
	Start = End + HeightTraceValue;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(Player);
	TArray<FHitResult> HitResults;
	FHitResult HitResult;

	UKismetSystemLibrary::CapsuleTraceMulti(
		this,
		Start,
		End,
		20.f,
		10.f,
		LedgeTraceType,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResults,
		true
	);

	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		Start,
		End,
		20.f,
		10.f,
		LedgeTraceType,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		if (isLeft)
		{
			LeftLedgeHeight = HitResults.Last().ImpactPoint;
		}
		else
		{
			RightLedgeHeight = HitResults.Last().ImpactPoint;
		}

		SetSidePoint(isLeft);

		return true;
	}
	else
	{
		if (isLeft)
		{
			LeftLedgeHeight = FVector::Zero();
		}
		else
		{
			RightLedgeHeight = FVector::Zero();
		}
	}

	return false;
}

void UClimbingComponent::GrabLedge()
{
	if (!Grabbing)
	{
		if (bGrabLedge) return;
		SetPoint();
		bGrabLedge = true;
		Grabbing = true;
		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		MovePlayerComponentTo();
	}
}

void UClimbingComponent::Hanging()
{
	FindSideLedge(true, Player->LeftArrow);
	FindSideLedge(false, Player->RightArrow);
}

void UClimbingComponent::FindSideLedge(bool isLeft, UArrowComponent* Arrow)
{
	if (LedgeGrabForwardTrace(isLeft, Arrow))
	{
		if (LedgeGrabHeightTrace(isLeft, Arrow))
		{
			if (isLeft)
			{
				Player->bCanMoveLeft = true;
			}
			else
			{
				Player->bCanMoveRight = true;
			}

		
		}
	}
	else
	{
		//Player->GetMovementComponent()->StopMovementImmediately();
		if (isLeft)
		{
			Player->bCanMoveLeft = false;

			LeftLedgeHeight = LedgeHeight;
		}
		else
		{
			Player->bCanMoveRight = false;
			RightLedgeHeight = LedgeHeight;
		}
	}

}

void UClimbingComponent::FullBraceTrace()
{
	FVector Start = Player->GetActorLocation() - FVector(0.f, 0.f, 20.f);
	FVector End = Player->GetActorForwardVector() * 90.f + Start;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(Player);
	FHitResult HitResult;

	UKismetSystemLibrary::SphereTraceSingle(
		this,
		Start,
		End,
		10.f,
		LedgeTraceType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	FullHang = !HitResult.bBlockingHit;
	LedgeWallWidth = (HitResult.Location - LedgeLocation).Length();

}

bool UClimbingComponent::RoomCheck()
{
	const FVector Start = 
		UKismetMathLibrary::GetForwardVector(
		UKismetMathLibrary::MakeRotFromX(LedgeNormal)) * -25.f +
		FVector(LedgeHeight.X, LedgeHeight.Y, LedgeHeight.Z + 60.f);
	const FVector End = Start;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(Player);

	FHitResult HitResult;

	UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		Start,
		End,
		30.f,
		30.f,
		LedgeTraceType,
		true,
		ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit)
	{
		CanStand = false;
	}
	else
	{
		if (LedgeWallWidth > StandableWallThreshold)
		{
			CanStand = true;
		}
	}

	//canstand check
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Bool: %s"), CanStand ? TEXT("true") : TEXT("false")));
	}//


	return CanStand;
}

void UClimbingComponent::SetPoint()
{
	FRotator Rotation1(FRotator(-LedgeNormal.Rotation().Pitch, LedgeNormal.Rotation().Yaw - 180.f, LedgeNormal.Rotation().Roll));
	ReferencePoint = 
		GetWorld()->SpawnActor<AReferencePoint>(
			ReferencePointClass,
			LedgeHeight,
			Rotation1
		);
	ReferencePoint->AttachToComponent(FResult.GetComponent(), AttachmentRules, FResult.BoneName);

}

void UClimbingComponent::SetSidePoint(bool IsLeft)
{
	if (IsLeft)
	{
		FRotator Rotation2(FRotator(-LeftLedgeNormal.Rotation().Pitch, LeftLedgeNormal.Rotation().Yaw - 180.f, LeftLedgeNormal.Rotation().Roll));
		LeftSidePoint =
			GetWorld()->SpawnActor<AReferencePoint>(
				ReferencePointClass,
				LeftLedgeHeight,
				Rotation2
			);
		LeftSidePoint->AttachToComponent(FResult.GetComponent(), AttachmentRules, FResult.BoneName);
	
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Bool: %s"), *LeftSidePoint->GetActorLocation().ToString()));
		}
	}
	else
	{
		FRotator Rotation3(FRotator(-RightLedgeNormal.Rotation().Pitch, RightLedgeNormal.Rotation().Yaw - 180.f, RightLedgeNormal.Rotation().Roll));
		RightSidePoint =
			GetWorld()->SpawnActor<AReferencePoint>(
				ReferencePointClass,
				RightLedgeHeight,
				Rotation3
			);
		RightSidePoint->AttachToComponent(FResult.GetComponent(), AttachmentRules, FResult.BoneName);
	}
}

void UClimbingComponent::MoveNextPoint()
{
	if (Player->MovingLeftRight < 0)
	{
		MovingLeft = true;
		MovingRight = false;
	}
	else if (Player->MovingLeftRight > 0)
	{
		MovingLeft = false;
		MovingRight = true;
	}

	MovingSideLedge = true;
	MovePlayerSideLedge();
}

void UClimbingComponent::ClimbUp()
{
	if (!ClimbingUp)
	{
		ClimbingUp = true;
		ReleaseLedge(EExitLedgeState::EEL_StillClimb);
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]()
			{
				ReleaseLedge(EExitLedgeState::EEL_Grounded);
				ClimbingUp = false;
			}
		), 1.0f, false);
	}
}

void UClimbingComponent::MovePlayerComponentTo()
{
	float NormalOffset = 30.f;
	float HeightOffset = -105.f;
	FVector Location = (ReferencePoint->GetActorLocation() + (LedgeNormal * NormalOffset));
	FRotator Rotation(FRotator(-LedgeNormal.Rotation().Pitch, LedgeNormal.Rotation().Yaw - 180.f, LedgeNormal.Rotation().Roll));

	Player->GetCapsuleComponent()->SetWorldRotation(Rotation, false);

	TEnumAsByte<EMoveComponentAction::Type> Action = EMoveComponentAction::Move;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "MovePlayerComponentFinished";
	LatentInfo.UUID = GetUniqueID();
	LatentInfo.Linkage = 0;

	float LedgeJumpTime;

	if (MovingSideLedge)
	{
		LedgeJumpTime = 0.01f;
	}
	else
	{
		LedgeJumpTime = 0.f;
	}
	

	UKismetSystemLibrary::MoveComponentTo(
		Player->GetCapsuleComponent(),
		FVector(Location.X, Location.Y, ReferencePoint->GetActorLocation().Z + HeightOffset),
		Rotation,
		true,
		true,
		LedgeJumpTime,
		true,
		Action,
		LatentInfo
	);
}

void UClimbingComponent::MovePlayerSideLedge()
{
	TEnumAsByte<EMoveComponentAction::Type> Action = EMoveComponentAction::Move;
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "MovePlayersideLedgeFinished";
	LatentInfo.UUID = GetUniqueID();
	LatentInfo.Linkage = 0;

	double LedgeJumpTime = .75f;

	if (Player->MovingLeftRight < 0)
	{
		UKismetSystemLibrary::MoveComponentTo(
			ReferencePoint->GetRootComponent(),
			LeftSidePoint->GetRootComponent()->GetRelativeLocation(),
			LeftSidePoint->GetRootComponent()->GetRelativeRotation(),
			true,
			true,
			LedgeJumpTime,
			true,
			Action,
			LatentInfo
		);
	}
	else if (Player->MovingLeftRight > 0)
	{
		UKismetSystemLibrary::MoveComponentTo(
			ReferencePoint->GetRootComponent(),
			RightSidePoint->GetRootComponent()->GetRelativeLocation(),
			RightSidePoint->GetRootComponent()->GetRelativeRotation(),
			true,
			true,
			LedgeJumpTime,
			true,
			Action,
			LatentInfo
		);
	}
}

void UClimbingComponent::MovePlayerComponentFinished()
{	
	if (ClimbState == EClimbState::ECS_Hanging) return;

	Player->GetCharacterMovement()->StopMovementImmediately();
	

	/*Player->GetMesh()->SetCollisionObjectType(ECC_PhysicsBody);
	Player->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	Player->PhysicalAnimationSettings();
	Player->GetMesh()->SetAllBodiesBelowSimulatePhysics("Pelvis", true, true);
	Player->GetMesh()->SetPhysicsBlendWeight(0.3f);
	Player->GetMesh()->SetAllBodiesBelowSimulatePhysics("clavicle_l", false, false);
	Player->GetMesh()->SetAllBodiesBelowSimulatePhysics("clavicle_r", false, false);
	Player->GetMesh()->SetAllBodiesBelowSimulatePhysics("neck_01", false, true);*/
	
	//Player->AttachToComponent(ReferencePoint->GetRootComponent(), AttachmentRules);
	ReferencePoint->SetPlayer(Player);

	FTimerHandle Timer1;
	FTimerHandle Timer2;

	//SetClimbState(EClimbState::ECS_Hanging);

	//bReleaseLedge = false;
	//Grabbing = false;

	GetWorld()->GetTimerManager().SetTimer(Timer1, FTimerDelegate::CreateLambda([&]()
		{
			bReleaseLedge = false;
		}
	), .5f, false);

	GetWorld()->GetTimerManager().SetTimer(Timer2, FTimerDelegate::CreateLambda([&]()
		{
			SetClimbState(EClimbState::ECS_Hanging);
			Grabbing = false;
			//1. 그랩이 끝난 직후 hanging으로 사이드 렛지 체크
			Hanging();
		}
	), .5f, false);
}

void UClimbingComponent::MovePlayersideLedgeFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("MPSLF"));
	if (!MovingSideLedge) return;

	MovingSideLedge = false;
	MovingLeft = false;
	MovingRight = false;
	if (SelectedSidePoint) SelectedSidePoint->Clear();
	if (LeftSidePoint) LeftSidePoint->Clear();
	if (RightSidePoint) RightSidePoint->Clear();
	Hanging();

	//FTimerHandle Timer1;
	//GetWorld()->GetTimerManager().SetTimer(Timer1, FTimerDelegate::CreateLambda([&]()
	//	{
	//		/*MovingSideLedge = false;
	//		MovingLeft = false;
	//		MovingRight = false;
	//		if (SelectedSidePoint) SelectedSidePoint->Clear();
	//		if (LeftSidePoint) LeftSidePoint->Clear();
	//		if (RightSidePoint) RightSidePoint->Clear();
	//		Hanging();*/
	//	}
	//), .585f, false);
}

void UClimbingComponent::ReleaseLedge(EExitLedgeState State)
{
	if(SelectedSidePoint) SelectedSidePoint->Clear();
	if(LeftSidePoint) LeftSidePoint->Clear();
	if(RightSidePoint) RightSidePoint->Clear();

	if (!ReferencePoint) return;
	ReferencePoint->Clear();
	
	ExitLedgeState = State;
	bReleaseLedge = true;

	Player->GetMesh()->SetCollisionObjectType(ECC_Pawn);
	Player->GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Player->GetMesh()->SetAllBodiesSimulatePhysics(false);

	ClimbState = EClimbState::ECS_ReleasedLedge;

	switch (ExitLedgeState)
	{
	case EExitLedgeState::EEL_InAir:
		Player->SetMovementState(EALSMovementState::InAir);
		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
		ClimbState = EClimbState::ECS_FindLedge;
		bGrabLedge = false;
		break;
	case EExitLedgeState::EEL_Grounded:
		Player->SetMovementState(EALSMovementState ::Grounded);
		Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
		ClimbState = EClimbState::ECS_FindLedge;
		bGrabLedge = false;
		break;
	case EExitLedgeState::EEL_StillClimb:

		break;
	}
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, FTimerDelegate::CreateLambda([&]()
		{
			
		}
	), .01f, false);

}


void UClimbingComponent::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<AALSCharacter>(GetOwner());
}

void UClimbingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ForwardTrace(FVector(0.f, 0.f, 70.f)))
	{
		if (HeightTrace())
		{
			if (!MovingSideLedge)
			{

			}

		}
	}

	if (ClimbState == EClimbState::ECS_ClimbUp)
	{
		ClimbUp();
	}

	if (!(WallCheck && HeightCheck)) return;

	if (Player->GetCharacterMovement()->IsFalling() || Player->GetMovementState() == EALSMovementState::Climb)
	{
		if (ClimbState == EClimbState::ECS_FindLedge)
		{

		}
		else if (ClimbState == EClimbState::ECS_GrabLedge && InputGrabPressed && WallCheck && HeightCheck)
		{
			GrabLedge();
			Player->SetMovementAction(EALSMovementAction::None);
			Player->SetMovementState(EALSMovementState::Climb);
		}
		else if (ClimbState == EClimbState::ECS_Hanging && InputGrabPressed)
		{
			if (ForwardTrace(FVector(0.f, 0.f, 70.f)))
			{
				if (HeightTrace())
				{
					//MovePlayerComponentTo();
				}
			}
		}
		else
		{
			ReleaseLedge(EExitLedgeState::EEL_InAir);
		}
	}

}

