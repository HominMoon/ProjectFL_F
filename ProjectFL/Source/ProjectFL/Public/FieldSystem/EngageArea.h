#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerSphere.h"
#include "EngageArea.generated.h"

UCLASS()
class PROJECTFL_API AEngageArea : public ATriggerSphere
{
	GENERATED_BODY()
	
public:
	AEngageArea();

	UPROPERTY(BlueprintReadWrite)
	bool bIsPlayerEnter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ASeeker* Seeker;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AGolem* Golem;

	
};
