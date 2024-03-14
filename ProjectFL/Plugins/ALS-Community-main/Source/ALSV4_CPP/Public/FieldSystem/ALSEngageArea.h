#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ALSEngageArea.generated.h"

class AALSCharacter;
class AEnemyBase;
class UCapsuleComponent;


UCLASS()
class ALSV4_CPP_API AALSEngageArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AALSEngageArea();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere);
	UCapsuleComponent* Capsule;

public:
	UPROPERTY(BlueprintReadWrite)
	bool bIsPlayerEnter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AALSCharacter* Player;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AEnemyBase* Enemy;
};
