#include "Enemy/EnemyBase.h"
#include "Enemy/WeakPoint.h"
#include "Components/BoxComponent.h"
#include "AIController.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyBase::Eleminate()
{

}



void AEnemyBase::SetHealth()
{
	TArray<AActor*> Actors;
	GetAttachedActors(Actors);

	for (size_t i = 0; i < Actors.Num(); i++)
	{
		if (Cast<AWeakPoint>(Actors[i]))
		{
			WeakPoints.Add(Cast<AWeakPoint>(Actors[i]));
		}
	}

	MaxHealth = WeakPoints.Num();
	Health = MaxHealth;

	UE_LOG(LogTemp, Warning, TEXT("%d %d"), MaxHealth, Health);

	return;
}

bool AEnemyBase::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;

	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemyBase::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(PatrolTarget);
	MoveRequest.SetAcceptanceRadius(20.f);

	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemyBase::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];

	}
	return nullptr;
}

void AEnemyBase::PatrolTimerFinished()
{
	IsWalking = true;
	MoveToTarget(PatrolTarget);
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	SetHealth();

	EnemyController = Cast<AAIController>(GetController());

	MoveToTarget(PatrolTarget);

}
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		IsWalking = false;
		PatrolTarget = ChoosePatrolTarget();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::PatrolTimerFinished, 5.f);
	}

}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	

}

