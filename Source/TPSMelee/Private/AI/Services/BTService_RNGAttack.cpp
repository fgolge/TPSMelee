// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTService_RNGAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemy/BaseAIController.h"
#include "Characters/Enemy/EnemyStates.h"

UBTService_RNGAttack::UBTService_RNGAttack()
{
	NodeName = TEXT("RNG Attack");

	bNotifyTick = true;

	DistanceToTargetKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_RNGAttack, DistanceToTargetKey));
}

void UBTService_RNGAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		DistanceToTargetKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTService_RNGAttack::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ABaseAIController* AIController = Cast<ABaseAIController>(OwnerComp.GetAIOwner());
	if(!AIController) return;
	UBlackboardComponent* BlackboardComp = OwnerComp.GetAIOwner()->GetBlackboardComponent();
	if(!BlackboardComp) return;

	if(AIController->IsStrafing())
	{
		if(ShouldAttack())
		{
			BlackboardComp->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, AIController->GetDistanceToTarget());
			AIController->SetEngagedState(EEnemyEngagedState::EEAS_Attack);
		}
		
	}

	
}

bool UBTService_RNGAttack::ShouldAttack()
{
	return FMath::FRandRange(0.f, 1.f) <= RNGFraction;
}