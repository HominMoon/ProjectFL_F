// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReferencePoint.generated.h"

UCLASS()
class ALSV4_CPP_API AReferencePoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReferencePoint();

	UPROPERTY(BlueprintReadWrite)
	class AALSCharacter* HangingPlayer;

	UFUNCTION(BlueprintImplementableEvent)
	void SetPlayer(AALSCharacter* Player);
	UFUNCTION(BlueprintImplementableEvent)
	void Clear();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class UCapsuleComponent* Capsule;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
