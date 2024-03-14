// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionPoint.generated.h"

class UBoxComponent;
class AALSCharacter;
class AEnemyBase;

UCLASS()
class ALSV4_CPP_API AInteractionPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractionPoint();

	virtual void PlayerRequestInteract();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AALSCharacter* Player;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//AEnemyBase* Base;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsPlayerIn = false;


};
