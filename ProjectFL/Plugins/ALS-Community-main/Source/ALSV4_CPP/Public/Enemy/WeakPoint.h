#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionPoint.h"
#include "WeakPoint.generated.h"

UCLASS()
class ALSV4_CPP_API AWeakPoint : public AInteractionPoint
{
	GENERATED_BODY()
	
public:	
	AWeakPoint();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void PlayerRequestInteract() override;

	UFUNCTION()
	void DestroyThisPoint();

protected:
	virtual void BeginPlay() override;

private:


public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WasDestroyed;

};
