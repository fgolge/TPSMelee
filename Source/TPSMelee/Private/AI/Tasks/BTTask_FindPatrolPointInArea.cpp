// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_FindPatrolPointInArea.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/BaseEnemy.h"

UBTTask_FindPatrolPointInArea::UBTTask_FindPatrolPointInArea()
{
	NodeName = TEXT("Find Patrol Point In Area");

	PatrolPointKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_FindPatrolPointInArea, PatrolPointKey));
}

EBTNodeResult::Type UBTTask_FindPatrolPointInArea::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ABaseEnemy* BaseEnemy = Cast<ABaseEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if(!BaseEnemy) return EBTNodeResult::Failed;

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if(!BlackboardComponent) return EBTNodeResult::Failed;
	
	FVector Center = BaseEnemy->GetPatrolCenter();

	FNavLocation Location;
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	bool bIsSuccessful = NavSystem->GetRandomPointInNavigableRadius(Center, PatrolRadius, Location);

	if(!bIsSuccessful) return EBTNodeResult::Failed;
	
	BlackboardComponent->SetValueAsVector(PatrolPointKey.SelectedKeyName, Location.Location);
	return EBTNodeResult::Succeeded;
}

void UBTTask_FindPatrolPointInArea::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();

	if(ensure(BBAsset))
	{
		PatrolPointKey.ResolveSelectedKey(*BBAsset);
	}
}


