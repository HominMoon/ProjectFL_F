#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "EnemyGolem.generated.h"

class UBoxComponent;

UCLASS()
class ALSV4_CPP_API AEnemyGolem : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyGolem();

	UFUNCTION()
	void OnBox1OverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnBox1OverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	UBoxComponent* AttackBox1;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsPlayerInBox;
	
};
