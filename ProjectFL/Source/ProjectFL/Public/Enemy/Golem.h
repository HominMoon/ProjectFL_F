#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Golem.generated.h"

class ASeeker;
class ATargetPoint;

UCLASS()
class PROJECTFL_API AGolem : public ACharacter
{
	GENERATED_BODY()

public:
	AGolem();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void PlayerInArea();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerOutArea();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerAttached();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerDettached();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerInRange();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayerOutRange();
	UFUNCTION(BlueprintImplementableEvent)
	void SetPlayer();
	UFUNCTION(BlueprintImplementableEvent)
	void SetBasePoint();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* RangeCapsule;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* RangeBox;

public:

	UFUNCTION(BlueprintCallable)
	void CallInArea();
	UFUNCTION(BlueprintCallable)
	void CallOutArea();
	UFUNCTION(BlueprintCallable)
	void CallInRange();
	UFUNCTION(BlueprintCallable)
	void CallOutRange();
	UFUNCTION(BlueprintCallable)
	void CallSetPlayer(ASeeker* Player);
	UFUNCTION(BlueprintCallable)
	void CallSetBasePoint();

	UPROPERTY(BlueprintReadWrite)
	ASeeker* Seeker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATargetPoint* BasePoint;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsPlayerInArea = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsPlayerInRange = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsPlayerHanging = false;


};
