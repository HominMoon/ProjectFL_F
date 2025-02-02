
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ClimberStates.h"
#include "Climber.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class APlayerController;

UCLASS()
class PROJECTFL_API AClimber : public ACharacter
{
	GENERATED_BODY()

public:
	AClimber();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void HipToLedge();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EPlayerMoveState MoveState = EPlayerMoveState::EPM_Idle;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	UCharacterMovementComponent* MovementComponent;

	APlayerController* CurrentPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
		UInputAction* JumpAction;

	UFUNCTION(BlueprintCallable)
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void ForwardTracer();
	void HeightTracer();
	void LeftTracer();
	void RightTracer();
	void JumpLeftTracer();
	void JumpRightTracer();

	UFUNCTION(BlueprintCallable)
	void GrabLedge();
	UFUNCTION(BlueprintCallable)
	void MoveInLedge();
	UFUNCTION(BlueprintCallable)
	void ExitLedge();

	UFUNCTION(BlueprintCallable)
	void ClimbLedge();
	UFUNCTION(BlueprintCallable)
	FVector GrabLedgeMoveToLocation();
	UFUNCTION(BlueprintCallable)
	FRotator GrabLedgeMoveToRotation();
	UFUNCTION(BlueprintCallable)
	void GrabLedgeMoveToLocationFinished();
	UFUNCTION(BlueprintCallable)
	void JumpSideLedge();
	UFUNCTION(BlueprintCallable)
	void Heal();

	void PlayClimbMontage();
	void PlayJumpSideLedgeMontage();

private:

	FHitResult FResult;
	const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
	FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(AttachmentRules, true);

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* LeftArrow;
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* RightArrow;
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* LeftLedgeArrow;
	UPROPERTY(VisibleAnywhere)
		UArrowComponent* RightLedgeArrow;


	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* EquipMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* ClimbMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
		UAnimMontage* JumpSideLedgeMontage;

	FVector WallLocation;
	FVector WallNormal;
	FVector HeightLocation;

	ETraceTypeQuery LedgeTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f))
	float Health = 100.f;
	

	UPROPERTY(BlueprintReadWrite)
	float fPressJumpTime = 0.f;

	UPROPERTY(BlueprintReadWrite)
	bool bIsHanging = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsClimbingLedge = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanGrab = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanMoveLeft = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanMoveRight = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanJumpLeft = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanJumpRight = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsMovingLeft = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsMovingRight = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsLedgeJumping = false;
	UPROPERTY(BlueprintReadWrite)
	bool WallChecked = false;
	UPROPERTY(BlueprintReadWrite)
	bool HeightChecked = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsTurnBack = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsJumpHigh = false;
};
