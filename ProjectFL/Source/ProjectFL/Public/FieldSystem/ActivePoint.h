#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementState.h"
#include "ActivePoint.generated.h"

class AActiveGround;
class ASavePoint;

UCLASS()
class PROJECTFL_API AActivePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AActivePoint();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void ActiveAllArea();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;

public:	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EElementState ElementState = EElementState::EES_None;

	UPROPERTY(BlueprintReadWrite)
	bool IsActivated;

	UPROPERTY(EditAnywhere)
	TArray<AActiveGround*> ActiveGrounds;

	UPROPERTY(EditAnywhere)
	ASavePoint* SavePoint;
};
