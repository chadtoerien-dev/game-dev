#pragma once

#include "CoreMinimal.h"
#include "TVGameplayEnums.generated.h"

UENUM(BlueprintType)
enum class ETheVeilPhase : uint8
{
    None,
    DawnAssembly,
    SocialFreeRoam,
    Challenge,
    Lobbying,
    Tribunal,
    Vote,
    Exile,
    NightPlanning,
    NightOperation,
    Aftermath
};

UENUM(BlueprintType)
enum class ETVOpportunityState : uint8
{
    Unknown,
    Hinted,
    Available,
    Prepared,
    Compromised,
    Completed,
    Abandoned
};

UENUM(BlueprintType)
enum class ETVAccessDecision : uint8
{
    Authorised,
    Tolerated,
    Unusual,
    Questionable,
    Trespassing,
    Hostile
};

UENUM(BlueprintType)
enum class ETVEventAudience : uint8
{
    DirectParticipants,
    LocalSenses,
    PublicBroadcast
};

UENUM(BlueprintType)
enum class ETVSensoryChannel : uint8
{
    Visual,
    Auditory,
    Mixed,
    Public
};

UENUM(BlueprintType)
enum class ETVInformationAcquisitionMethod : uint8
{
    Witnessed,
    Heard,
    Inferred,
    Reported,
    Rumour,
    PhysicalEvidence,
    PublicAnnouncement,
    Fabricated
};

UENUM(BlueprintType)
enum class ETVInformationTransmissionMode : uint8
{
    Report,
    Rumour,
    DeliberateLie
};

UENUM(BlueprintType)
enum class ETVTruthAssessment : uint8
{
    Unknown,
    BelievedTrue,
    Questioned,
    BelievedFalse
};

UENUM(BlueprintType)
enum class ETVClaimVisibility : uint8
{
    Private,
    Limited,
    Public
};

UENUM(BlueprintType)
enum class ETVClaimStance : uint8
{
    Assert,
    Deny,
    Accuse,
    Defend,
    Support,
    Retract
};

UENUM(BlueprintType)
enum class ETVContradictionSeverity : uint8
{
    None,
    Minor,
    Material,
    Critical
};

UENUM(BlueprintType)
enum class ETVVoteIntentStatus : uint8
{
    Undecided,
    Leaning,
    Committed,
    Locked
};
