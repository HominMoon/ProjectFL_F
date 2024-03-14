#pragma once

UENUM(BlueprintType)
enum class EClimbState : uint8
{
	ECS_FindLedge UMETA(DisplayName = "FindLedge"),
	ECS_GrabLedge UMETA(DisplayName = "GrabLedge"),
	ECS_Hanging UMETA(DisplayName = "Hanging"),
	ECS_ClimbUp UMETA(DisplayName = "ClimbUp"),
	ECS_ReleasedLedge UMETA(DisplayName = "ReleasedLedge"),
	ECS_JumpingUp UMETA(DisplayName = "JumpingUp"),
	ECS_Turning UMETA(DisplayName = "Turning")

};

UENUM(BlueprintType)
enum class EExitLedgeState : uint8
{
	EEL_None UMETA(DisplayName = "None"),
	EEL_InAir UMETA(DisplayName = "InAir"),
	EEL_Grounded UMETA(DisplayName = "Grounded"),
	EEL_StillClimb UMETA(DisplayName = "StillClimb")
};