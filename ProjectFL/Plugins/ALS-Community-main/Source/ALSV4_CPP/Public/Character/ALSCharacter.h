// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#pragma once

#include "CoreMinimal.h"
#include "Character/ALSBaseCharacter.h"
#include "ALSCharacter.generated.h"

class UArrowComponent;
class AInteractionPoint;

UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AALSCharacter : public AALSBaseCharacter
{
	GENERATED_BODY()

public:
	AALSCharacter(const FObjectInitializer& ObjectInitializer);

	/** Implemented on BP to update held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void ClearHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh,
	                  class UClass* NewAnimClass, bool bLeftHand, FVector Offset);

	virtual void RagdollStart() override;

	virtual void RagdollEnd() override;

	//Add Fall Damage System
	virtual void EventOnLanded() override;

	//Custom TakeDamage
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	virtual FVector GetFirstPersonCameraTarget() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PhysicalAnimationSettings();

	//PlayerInteraction
	UFUNCTION()
	void SetOverlappingPoint(AInteractionPoint* Point);
	UFUNCTION(BlueprintCallable)
	void BeginCharge();
	UFUNCTION(BlueprintCallable)
	void EndCharge();	
	UFUNCTION(BlueprintCallable)
	void Interaction();
protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(EALSOverlayState PreviousState) override;

	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObjectAnimations();

	void Heal();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<USceneComponent> HeldObjectRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;

	//Custom TakeDamage
	TSubclassOf<UDamageType> DamageType;


	// Custom Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f), Category = "Health")
	float Health = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float HealAmount = 1.f;

	//Custom MoveArrowLR
	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* LeftArrow;
	UPROPERTY(EditDefaultsOnly)
	UArrowComponent* RightArrow;


	// Custom Fall Damage&RagdollTime
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RagdollOnLandDamage = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RagdollTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AInteractionPoint* OverlappingPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttackCharged = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CanInteraction = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BeginChargeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EndChargeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxChargeTime = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChargeTime = 0.f;

private:
	bool bNeedsColorReset = false;
};
