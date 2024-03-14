#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ElementState.h"
#include "Seeker.generated.h"


class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class APlayerController;

class AGolem;
class ASavePoint;
class AActivePoint;

UCLASS()
class PROJECTFL_API ASeeker : public ACharacter
{
	GENERATED_BODY()

public:
	ASeeker();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


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

	UFUNCTION(BlueprintCallable)
	void GrabLedge();
	UFUNCTION(BlueprintCallable)
	void FixedToWall();
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
	FVector FixedToWallMoveToLocation();
	UFUNCTION(BlueprintCallable)
	FRotator FixedToWallMoveToRotation();
	UFUNCTION(BlueprintCallable)
	void FixedToWallMoveToLocationFinished();

	// for calculate distance between ledge to player character spine
	UFUNCTION(BlueprintCallable)
	void HipToLedge(); 

	//how to work: if (wallchecked true -> if (heightchecked true -> GrabLedge false -> FixedToWall))

	UFUNCTION(BlueprintCallable)
	void Heal();

	void PlayClimbMontage();

private:

	FHitResult FResult;
	const FAttachmentTransformRules AttachmentRules = FAttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	FDetachmentTransformRules DetachmentRules = FDetachmentTransformRules(AttachmentRules, true);

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ClimbMontage;

	FVector WallLocation;
	FVector HeightLocation;

	ETraceTypeQuery LedgeTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f))
	float Health = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHighJumpStamina = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HighJumpStamina = 5.f;

	UPROPERTY(BlueprintReadWrite)
	float fPressJumpTimer = 0.f;

	UPROPERTY(BlueprintReadWrite)
	ASavePoint* LastSavePoint;
	UPROPERTY(BlueprintReadWrite)
	AActivePoint* LastActivePoint;

	UPROPERTY(BlueprintReadWrite)
	EElementState ElementState = EElementState::EES_None;

	//******************************
	UPROPERTY(BlueprintReadOnly)
		FVector WallNormal;
	//************************

	UPROPERTY(BlueprintReadWrite)
	bool bIsHanging = false;
	UPROPERTY(BlueprintReadWrite)
	bool bWasFixed = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsClimbingLedge = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanGrab = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanFix = false;
	UPROPERTY(BlueprintReadOnly)
	bool bCanActivate = false;
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
	UPROPERTY(BlueprintReadWrite)
	bool bIsRagdoll = false;
};
