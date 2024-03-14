
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ElementState.h"
#include "SavePoint.generated.h"

UCLASS()
class PROJECTFL_API ASavePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASavePoint();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void SaveThisPoint();

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* CollisionBox;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	bool IsActivated;

};
