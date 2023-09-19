#pragma once

UENUM(BlueprintType)
enum class EEnemyActionState : uint8
{
	EEAS_NoState UMETA(DisplayName = "No State"),
	EEAS_Patrolling UMETA(DisplayName = "Patrolling"),
	EEAS_Investigating UMETA(DisplayName = "Investigating"),
	EEAS_Chasing UMETA(DisplayName = "Chasing"),
	EEAS_Engaged UMETA(DisplayName = "Engaged"),
	EEAS_HitReaction UMETA(DisplayName = "Hit Reaction"),
	EEAS_Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EEnemyEngagedState : uint8
{
	EEAS_Strafe UMETA(DisplayName = "Strafe"),
	EEAS_Attack UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)
enum class EEnemySpeedState : uint8
{
	EESS_PatrolSpeed UMETA(DisplayName = "Patrolling Speed"),
	EESS_InvestigateSpeed UMETA(DisplayName = "Investigating Speed"),
	EESS_ChaseSpeed UMETA(DisplayName = "Chasing Speed"),
	EESS_EngagedSpeed UMETA(DisplayName = "Engaged Speed")
};