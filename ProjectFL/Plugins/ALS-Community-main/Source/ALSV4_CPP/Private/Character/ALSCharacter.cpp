// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#include "Character/ALSCharacter.h"
#include "Enemy/WeakPoint.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ClimbingComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/StaticMesh.h"
#include "AI/ALSAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"



AALSCharacter::AALSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeldObjectRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HeldObjectRoot"));
	HeldObjectRoot->SetupAttachment(GetMesh());

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(HeldObjectRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(HeldObjectRoot);

	AIControllerClass = AALSAIController::StaticClass();

	ClimbingComp = CreateDefaultSubobject<UClimbingComponent>(TEXT("ClimbingComponent"));

	LeftArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Left Arrow"));
	LeftArrow->SetupAttachment(GetRootComponent());
	RightArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Right Arrow"));
	RightArrow->SetupAttachment(GetRootComponent());

}

void AALSCharacter::ClearHeldObject()
{
	StaticMesh->SetStaticMesh(nullptr);
	SkeletalMesh->SetSkeletalMesh(nullptr);
	SkeletalMesh->SetAnimInstanceClass(nullptr);
}

void AALSCharacter::AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, UClass* NewAnimClass,
                                 bool bLeftHand, FVector Offset)
{
	ClearHeldObject();

	if (IsValid(NewStaticMesh))
	{
		StaticMesh->SetStaticMesh(NewStaticMesh);
	}
	else if (IsValid(NewSkeletalMesh))
	{
		SkeletalMesh->SetSkeletalMesh(NewSkeletalMesh);
		if (IsValid(NewAnimClass))
		{
			SkeletalMesh->SetAnimInstanceClass(NewAnimClass);
		}
	}

	FName AttachBone;
	if (bLeftHand)
	{
		AttachBone = TEXT("VB LHS_ik_hand_gun");
	}
	else
	{
		AttachBone = TEXT("VB RHS_ik_hand_gun");
	}

	HeldObjectRoot->AttachToComponent(GetMesh(),
	                                  FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachBone);
	HeldObjectRoot->SetRelativeLocation(Offset);
}

void AALSCharacter::RagdollStart()
{
	ClearHeldObject();
	Super::RagdollStart();

	//for custom ragdoll recovery(getup)
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AALSBaseCharacter::RagdollEnd, RagdollTime, false);
	//
}

void AALSCharacter::RagdollEnd()
{
	Super::RagdollEnd();
	UpdateHeldObject();
}


void AALSCharacter::EventOnLanded()
{
	Super::EventOnLanded();

	// for custom fall damage event
	int32 Damage = UKismetMathLibrary::Round(
					UKismetMathLibrary::Clamp(
					UKismetMathLibrary::NormalizeToRange(
					UKismetMathLibrary::Abs(
					LandedVelocity), 1500.f, 3000.f)* 10.f, 0.f, 10.f));

	float var = UGameplayStatics::ApplyDamage(this, Damage, nullptr, nullptr, UDamageType::StaticClass());

	RagdollTime = UKismetMathLibrary::Clamp(Damage, 2.f, 7.f);
	UE_LOG(LogTemp, Warning, TEXT("%f"), RagdollTime);

	if (Damage >= RagdollOnLandDamage)
	{
		ReplicatedRagdollStart();
	}
	//
}

float AALSCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return Damage;
}

ECollisionChannel AALSCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	const FName CameraSocketName = bRightShoulder ? TEXT("TP_CameraTrace_R") : TEXT("TP_CameraTrace_L");
	TraceOrigin = GetMesh()->GetSocketLocation(CameraSocketName);
	TraceRadius = 15.0f;
	return ECC_Camera;
}

FTransform AALSCharacter::GetThirdPersonPivotTarget()
{
	return FTransform(GetActorRotation(),
	                  (GetMesh()->GetSocketLocation(TEXT("Head")) + GetMesh()->GetSocketLocation(TEXT("root"))) / 2.0f,
	                  FVector::OneVector);
}

FVector AALSCharacter::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(TEXT("FP_Camera"));
}

void AALSCharacter::OnOverlayStateChanged(EALSOverlayState PreviousState)
{
	Super::OnOverlayStateChanged(PreviousState);
	UpdateHeldObject();
}

void AALSCharacter::Heal()
{
	if (Health < 10.f)
	{
		float var = UGameplayStatics::ApplyDamage(this, -HealAmount, nullptr, nullptr, UDamageType::StaticClass());
	}

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &AALSCharacter::Heal, HealTime, false);
}


void AALSCharacter::SetOverlappingPoint(AInteractionPoint* Point)
{
	OverlappingPoint = Point;
}

void AALSCharacter::BeginCharge()
{
	BeginChargeTime = FApp::GetCurrentTime();
	UE_LOG(LogTemp, Warning, TEXT("!"));
}

void AALSCharacter::EndCharge()
{
	EndChargeTime = FApp::GetCurrentTime();

	ChargeTime = EndChargeTime - BeginChargeTime + 0.1f;
	UE_LOG(LogTemp, Warning, TEXT("%f"), ChargeTime);

	if (ChargeTime >= MaxChargeTime)
	{
		IsAttackCharged = true;
		CanInteraction = true;
	}
	else if (ChargeTime < MaxChargeTime && ChargeTime >= 0.f)
	{
		IsAttackCharged = false;
		CanInteraction = true;
	}

	Interaction();

}

void AALSCharacter::Interaction()
{
	OverlappingPoint->PlayerRequestInteract();
}

void AALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHeldObjectAnimations();
}

void AALSCharacter::BeginPlay()
{
	Super::BeginPlay();

	Heal();

	UpdateHeldObject();
}
