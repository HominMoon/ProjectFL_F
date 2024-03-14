// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ClimbState.h"
#include "Interfaces/ClimbInterface.h"
#include "ClimbingComponent.generated.h"

class AALSCharacter;
class UArrowComponent;
class AReferencePoint;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class ALSV4_CPP_API UClimbingComponent : public UActorComponent, public IClimbInterface
{
	GENERATED_BODY()

public:	
	UClimbingComponent();

	EClimbState GetClimbState() const { return ClimbState; }
	void SetClimbState(EClimbState NewState);

	virtual void ClimbValues_Implementation(EClimbState& OutState, bool& OutMovingLeft, bool& OutMovingRight, bool& OutFullHang) override;
	
	bool FullHang;
	bool CanStand;
	bool MovingSideLedge;

	void SetPoint();
	void SetSidePoint(bool IsLeft);
	void MoveNextPoint();

	UFUNCTION(BlueprintCallable)
	void ReleaseLedge(EExitLedgeState State);
	UFUNCTION(BlueprintCallable)
	bool RoomCheck();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AReferencePoint> ReferencePointClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AReferencePoint* ReferencePoint; //relative

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AReferencePoint* LeftSidePoint; //relative

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AReferencePoint* RightSidePoint; //relative

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AReferencePoint* SelectedSidePoint; //relative


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SelectedLedgeLocation; //relative

protected:
	virtual void BeginPlay() override;

	bool ForwardTrace(FVector HeightOffset);
	bool LedgeGrabForwardTrace(bool isLeft, UArrowComponent* Arrow);
	bool HeightTrace();
	bool LedgeGrabHeightTrace(bool isLeft, UArrowComponent* Arrow);

	void GrabLedge();
	void Hanging();
	void FindSideLedge(bool isLeft, UArrowComponent* Arrow);
	void FullBraceTrace();
	void ClimbUp();

	UFUNCTION(BlueprintCallable)
	void MovePlayerComponentTo();
	UFUNCTION(BlueprintCallable)
	void MovePlayerSideLedge();
	UFUNCTION(BlueprintCallable)
	void MovePlayerComponentFinished();
	UFUNCTION(BlueprintCallable)
	void MovePlayersideLedgeFinished();

	UPROPERTY(BlueprintReadWrite)
	AALSCharacter* Player;

	UPROPERTY(BlueprintReadWrite)
	EClimbState ClimbState = EClimbState::ECS_FindLedge;
	UPROPERTY(BlueprintReadWrite)
	EExitLedgeState ExitLedgeState = EExitLedgeState::EEL_Grounded;

	UPROPERTY(BlueprintReadWrite)
	bool InputGrabPressed;
	UPROPERTY(BlueprintReadWrite)
	bool WallCheck;
	UPROPERTY(BlueprintReadWrite)
	bool HeightCheck;

	UPROPERTY(BlueprintReadWrite)
	FVector LedgeNormal;
	UPROPERTY(BlueprintReadWrite)
	FVector LedgeLocation;
	UPROPERTY(BlueprintReadWrite)
	FVector LedgeHeight;

	UPROPERTY(BlueprintReadWrite)
	FVector LeftLedgeNormal;
	UPROPERTY(BlueprintReadWrite)
	FVector RightLedgeNormal;
	UPROPERTY(BlueprintReadWrite)
	FVector LeftLedgeLocation;
	UPROPERTY(BlueprintReadWrite)
	FVector RightLedgeLocation;
	UPROPERTY(BlueprintReadWrite)
	FVector LeftLedgeHeight;
	UPROPERTY(BlueprintReadWrite)
	FVector RightLedgeHeight;


private:

	FHitResult FResult;
	const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(AttachmentRules, true);

	float GrabbableLedgeTreshold = 250.f;
	float StandableWallThreshold = 0.f;
	float LedgeWallWidth;

	bool Grabbing;
	bool MovingLeft;
	bool MovingRight;
	bool ClimbingUp = false;
	
	//Reset grabLedge, ReleaseLedge for do once
	bool bGrabLedge;
	bool bReleaseLedge;

	ETraceTypeQuery LedgeTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel2);
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
