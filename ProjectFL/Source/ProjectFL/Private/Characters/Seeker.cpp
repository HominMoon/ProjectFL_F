// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Seeker.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/DefaultPawn.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"

#include "Enemy/Golem.h"
#include "FieldSystem/SavePoint.h"
#include "FieldSystem/ActivePoint.h"

#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
ASeeker::ASeeker()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void ASeeker::BeginPlay()
{
	Super::BeginPlay();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ASeeker::OnOverlapBegin);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ASeeker::OnOverlapEnd);

	// For EnhancedInput
	MovementComponent = GetCharacterMovement();
	CurrentPlayerController = Cast<APlayerController>(GetController());

	if (CurrentPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(CurrentPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	
}

// Called every frame
void ASeeker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ForwardTracer();
	HeightTracer();

	if (CurrentPlayerController)
	{
		if (bIsHanging || bWasFixed)
		{

		}
		else
		{
			DetachFromActor(DetachmentRules);
		}
	}

}

void ASeeker::Move(const FInputActionValue& Value)
{

	FVector2D MoveVector = Value.Get<FVector2D>();

	if (MoveVector.Length() < 0.05 || bIsHanging || bWasFixed) return;

	const FRotator ControlRotation = GetControlRotation();
	const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MoveVector.Y);
	AddMovementInput(RightDirection, MoveVector.X);
}

void ASeeker::Look(const FInputActionValue& Value)
{
	const FVector2D LookVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookVector.Y);
	AddControllerYawInput(LookVector.X);
}

void ASeeker::ForwardTracer()
{
	double ValueX = GetActorForwardVector().X * 150;
	double ValueY = GetActorForwardVector().Y * 150;
	FVector ForwardTraceValue(FVector(ValueX, ValueY, GetActorForwardVector().Z));
	const FVector Start = GetActorLocation() + FVector(0.f, 0.f, 70.f);
	const FVector End = Start + ForwardTraceValue;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
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
		WallLocation = HitResult.Location;
		WallNormal = HitResult.ImpactNormal;
		ActorsToIgnore.AddUnique(this);
		WallChecked = true;
	}
	else
	{
		WallChecked = false;
	}

	return;
}

void ASeeker::HeightTracer()
{
	FVector HeightTraceValue(FVector(0.f, 0.f, 200.f));
	FVector ForwardTraceValue = GetActorForwardVector() * 70.f;
	FVector End = GetActorLocation() + ForwardTraceValue;
	FVector Start = End + HeightTraceValue;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.AddUnique(this);
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
		FResult = HitResult;
		HeightLocation = HitResult.ImpactPoint;
		HeightChecked = true;
	}
	else
	{
		HeightChecked = false;
	}
}

void ASeeker::GrabLedge()
{
	if (!MovementComponent || bIsHanging || !WallChecked || !HeightChecked) return;

	MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	bIsHanging = true;
	TEnumAsByte<EMoveComponentAction::Type> Action = EMoveComponentAction::Move;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "GrabLedgeMoveToLocationFinished";
	LatentInfo.UUID = GetUniqueID();
	LatentInfo.Linkage = 0;

	UKismetSystemLibrary::MoveComponentTo(
		RootComponent,
		GrabLedgeMoveToLocation(),
		GrabLedgeMoveToRotation(),
		false,
		false,
		0.15f,
		false,
		Action,
		LatentInfo
	);
}

void ASeeker::FixedToWall()
{
	if (!MovementComponent || bIsHanging || !WallChecked) return;

	MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	bWasFixed = true;
	TEnumAsByte<EMoveComponentAction::Type> Action = EMoveComponentAction::Move;

	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "FixedToWallMoveToLocationFinished";
	LatentInfo.UUID = GetUniqueID();
	LatentInfo.Linkage = 0;

	UKismetSystemLibrary::MoveComponentTo(
		RootComponent,
		GrabLedgeMoveToLocation(),
		GrabLedgeMoveToRotation(),
		false,
		false,
		0.15f,
		false,
		Action,
		LatentInfo
	);
}

void ASeeker::ExitLedge()
{
	bIsHanging = false;
	bWasFixed = false;

	AGolem* Golem = Cast<AGolem>(FResult.GetActor());
	if (Golem)
	{
		Golem->PlayerDettached();
	}

	FResult.Reset();
	FRotator NewRotation(FRotator(0.f, GetActorRotation().Yaw, 0.f));
	SetActorRotation(NewRotation);
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
}

void ASeeker::ClimbLedge()
{
	if (bIsClimbingLedge) return;
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
	bIsClimbingLedge = true;
	bIsHanging = false;

	PlayClimbMontage();
}

FVector ASeeker::GrabLedgeMoveToLocation()
{
	double LocationX = WallLocation.X + WallNormal.X * 22.f;
	double LocationY = WallLocation.Y + WallNormal.Y * 22.f;
	double LocationZ;
	if (!HeightChecked || HeightLocation.Z < WallLocation.Z) LocationZ = WallLocation.Z;
	else LocationZ = HeightLocation.Z - 120.f;

	FVector LocationF(FVector(LocationX, LocationY, LocationZ));

	return LocationF;
}

FRotator ASeeker::GrabLedgeMoveToRotation()
{
	FRotator Rotation(FRotator(-WallNormal.Rotation().Pitch, WallNormal.Rotation().Yaw - 180.f, WallNormal.Rotation().Roll));
	return Rotation;
}

void ASeeker::GrabLedgeMoveToLocationFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("GrabLedge Finished"));
	MovementComponent->StopMovementImmediately();

	//AttachToActor(FResult.GetActor(), AttachmentRules);
	AttachToComponent(FResult.GetComponent(), AttachmentRules, FResult.BoneName);
	UE_LOG(LogTemp, Warning, TEXT("%s"), FResult.BoneName);

	AGolem* Golem = Cast<AGolem>(FResult.GetActor());
	if (Golem)
	{
		Golem->PlayerAttached();
	}
}

FVector ASeeker::FixedToWallMoveToLocation()
{
	double LocationX = WallLocation.X + WallNormal.X * 22.f;
	double LocationY = WallLocation.Y + WallNormal.Y * 22.f;
	double LocationZ = HeightLocation.Z - 120.f;

	FVector LocationF(FVector(LocationX, LocationY, LocationZ));

	return LocationF;
}

FRotator ASeeker::FixedToWallMoveToRotation()
{
	FRotator Rotation(FRotator(-WallNormal.Rotation().Pitch, WallNormal.Rotation().Yaw - 180.f, WallNormal.Rotation().Roll));
	return Rotation;
}

void ASeeker::FixedToWallMoveToLocationFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("FixedToWall Finished"));
	MovementComponent->StopMovementImmediately();

	//AttachToActor(FResult.GetActor(), AttachmentRules);
	AttachToComponent(FResult.GetComponent(), AttachmentRules, FResult.BoneName);
	UE_LOG(LogTemp, Warning, TEXT("%s"), FResult.BoneName);

	AGolem* Golem = Cast<AGolem>(FResult.GetActor());
	if (Golem)
	{
		Golem->PlayerAttached();
	}
}

void ASeeker::HipToLedge()
{
	if (bWasFixed || bIsHanging) return;

	double SpineLocationZ = GetMesh()->GetSocketLocation(FName(TEXT("SpineSocket"))).Z;
	double HeightLocationZ = HeightLocation.Z;

	double Range = SpineLocationZ - HeightLocationZ;

	if (UKismetMathLibrary::InRange_FloatFloat(Range, -70.f, 20.f) && !bIsClimbingLedge && WallChecked && HeightChecked)
	{
		bCanGrab = true;
		GrabLedge();
	}
	else
	{
		bCanGrab = false;
		bCanFix = true;
		FixedToWall();
	}

}


void ASeeker::Heal()
{
	if (Health >= 10.f) return;

	Health += 1.f;
}

void ASeeker::PlayClimbMontage()
{
	UAnimInstance* Instance = GetMesh()->GetAnimInstance();
	if (Instance && ClimbMontage)
	{
		Instance->Montage_Play(ClimbMontage);
	}
}

// Called to bind functionality to input
void ASeeker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASeeker::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASeeker::Look);

	}

}

void ASeeker::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && Cast<AActivePoint>(OtherActor))
	{
		LastActivePoint = Cast<AActivePoint>(OtherActor);
		bCanActivate = true;
	}
}

void ASeeker::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

