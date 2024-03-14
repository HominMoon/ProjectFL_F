#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

class AWeakPoint;
class AAIController;

UCLASS()
class ALSV4_CPP_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyBase();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Eleminate();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetHealth();

	UFUNCTION()
	bool InTargetRange(AActor* Target, double Radius);
	UFUNCTION()
	void MoveToTarget(AActor* Target);
	UFUNCTION()
	AActor* ChoosePatrolTarget();

private:

public:	
	//Enemy Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AWeakPoint*> WeakPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;


	//Enemy AI Control
	UPROPERTY()
	AAIController* EnemyController;
	UPROPERTY(EditAnywhere, Category = "AI Control")
	AActor* BasePoint;

	UPROPERTY(EditAnywhere, Category = "AI Control")
	AActor* PatrolTarget;
	UPROPERTY(EditAnywhere, Category = "AI Control")
	TArray<AActor*> PatrolTargets;
	UPROPERTY(EditAnywhere, Category = "AI Control")
	double PatrolRadius = 3000.f;

	FTimerHandle PatrolTimer;
	void PatrolTimerFinished();
	//Enemy States
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Control")
	bool IsPlayerEngage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Control")
	bool IsPlayerStepOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Control")
	bool IsWalking;
};
