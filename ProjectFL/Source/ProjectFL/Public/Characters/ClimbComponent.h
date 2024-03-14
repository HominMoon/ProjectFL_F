// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClimbState.h"
#include "Interfaces/ClimbInterface.h"
#include "ClimbComponent.generated.h"

class AALSCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTFL_API UClimbComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UClimbComponent();

	EClimbState GetClimbState() const { return ClimbState; }
	void SetClimbState(EClimbState NewState);

	//void ClimbValues_Implementation(EClimbState& State, bool& MovingLeft, bool& MovingRight);

protected:
	virtual void BeginPlay() override;

	bool ForwardTrace(FVector HeightOffset);
	bool HeightTrace();
	void GrabLedge();
	void MovePlayerComponentTo();
	void MovePlayerComponentFinished();

	EClimbState ClimbState = EClimbState::ECS_FindLedge;
	AALSCharacter* Player;

private:

	FVector LedgeNormal;
	FVector LedgeLocation;
	FVector LedgeHeight;
	float GrabbableLedgeTreshold = 250.f;
	bool Grabbing;
	bool MovingL;
	bool MovingR;

	ETraceTypeQuery LedgeTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
