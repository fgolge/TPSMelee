#pragma once

UENUM(BlueprintType)
enum class EEnemyActionState : uint8
{
	EEAS_NoState UMETA(DisplayName = "No State"),
	EEAS_Patrolling UMETA(DisplayName = "Patrolling"),
	EEAS_Investigating UMETA(DisplayName = "Investigating"),
	EEAS_Chasing UMETA(DisplayName = "Chasing"),
	EEAS_Attacking UMETA(DisplayName = "Attacking"),
	EEAS_HitReaction UMETA(DisplayName = "Hit Reaction"),
	EEAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EEnemySpeedState : uint8
{
	EESS_PatrolSpeed UMETA(DisplayName = "Patrolling Speed"),
	EESS_InvestigateSpeed UMETA(DisplayName = "Investigating Speed"),
	EESS_ChaseSpeed UMETA(DisplayName = "Chasing Speed"),
	EESS_AttackSpeed UMETA(DisplayName = "Attacking Speed"),
};