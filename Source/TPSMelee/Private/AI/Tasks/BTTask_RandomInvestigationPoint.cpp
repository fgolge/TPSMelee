// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Tasks/BTTask_RandomInvestigationPoint.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_RandomInvestigationPoint::UBTTask_RandomInvestigationPoint()
{
	NodeName = TEXT("Random Investigation Point");
}

EBTNodeResult::Type UBTTask_RandomInvestigationPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if(!BlackboardComponent) return EBTNodeResult::Failed;

	FVector Center = BlackboardComponent->GetValueAsVector(LastSeenKey.SelectedKeyName);
	
	FNavLocation Location;
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	bool bIsSuccessful = NavSystem->GetRandomPointInNavigableRadius(Center, SearchRadius, Location);

	if(!bIsSuccessful) return EBTNodeResult::Failed;

	BlackboardComponent->SetValueAsVector(InvestigationPointKey.SelectedKeyName, Location.Location);
	return EBTNodeResult::Succeeded;
}

void UBTTask_RandomInvestigationPoint::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();

	if(ensure(BBAsset))
	{
		LastSeenKey.ResolveSelectedKey(*BBAsset);
		InvestigationPointKey.ResolveSelectedKey(*BBAsset);
	}
}
