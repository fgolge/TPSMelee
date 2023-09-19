// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_UpdateLocationOfTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateLocationOfTarget::UBTService_UpdateLocationOfTarget()
{
	NodeName = TEXT("Update Location");

	bNotifyTick = true;
	
	Location.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateLocationOfTarget, Location));
	TargetActor.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateLocationOfTarget, TargetActor), AActor::StaticClass());
}

void UBTService_UpdateLocationOfTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		Location.ResolveSelectedKey(*BBAsset);
		TargetActor.ResolveSelectedKey(*BBAsset);
	}
}

void UBTService_UpdateLocationOfTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if (!BlackboardComp) return;

	AActor* TargetToUpdate = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetActor.SelectedKeyName));
	if(TargetToUpdate)
	{
		BlackboardComp->SetValueAsVector(Location.SelectedKeyName, TargetToUpdate->GetActorLocation());
	}
}


