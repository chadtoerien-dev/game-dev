#include "Memory/TVInformationSubsystem.h"
#include "Memory/TVInformationRules.h"
#include "Memory/TVMemoryComponent.h"
#include "GameFramework/Actor.h"

FTVInformationTransmissionResult UTVInformationSubsystem::TransmitMemory(AActor* SourceActor, AActor* RecipientActor, const FGuid SourceMemoryId, const ETVInformationTransmissionMode Mode, const FTVInformationMutation& Mutation, const int32 Seed)
{
    FTVInformationTransmissionResult Result;
    if (!IsValid(SourceActor) || !IsValid(RecipientActor))
    {
        Result.FailureReason = FText::FromString(TEXT("Source and recipient actors are required."));
        return Result;
    }
    UTVMemoryComponent* Source = SourceActor->FindComponentByClass<UTVMemoryComponent>();
    UTVMemoryComponent* Recipient = RecipientActor->FindComponentByClass<UTVMemoryComponent>();
    if (!IsValid(Source) || !IsValid(Recipient))
    {
        Result.FailureReason = FText::FromString(TEXT("Both actors require TVMemoryComponent."));
        return Result;
    }
    FTVMemoryRecord SourceMemory;
    if (!Source->FindMemory(SourceMemoryId, SourceMemory))
    {
        Result.FailureReason = FText::FromString(TEXT("The source memory could not be found."));
        return Result;
    }
    const FName SourceId = Source->ResolveOwnerContestantId();
    const FName RecipientId = Recipient->ResolveOwnerContestantId();
    Result = UTVInformationRules::BuildTransmittedMemory(SourceMemory, SourceId, RecipientId, Mode, Mutation, Seed);
    if (!Result.bSucceeded) return Result;
    const FGuid AddedId = Recipient->AddMemory(Result.Memory, true, true);
    if (!AddedId.IsValid())
    {
        Result.bSucceeded = false;
        Result.FailureReason = FText::FromString(TEXT("The recipient rejected the transmitted memory."));
        return Result;
    }
    Result.Memory.MemoryId = AddedId;
    OnInformationTransmitted.Broadcast(SourceId, RecipientId, Result.Memory);
    return Result;
}

TArray<FTVInformationTransmissionResult> UTVInformationSubsystem::BroadcastMemory(AActor* SourceActor, const TArray<AActor*>& Recipients, const FGuid SourceMemoryId, const ETVInformationTransmissionMode Mode, const FTVInformationMutation& Mutation, const int32 BaseSeed)
{
    TArray<FTVInformationTransmissionResult> Results;
    Results.Reserve(Recipients.Num());
    for (int32 Index = 0; Index < Recipients.Num(); ++Index)
        Results.Add(TransmitMemory(SourceActor, Recipients[Index], SourceMemoryId, Mode, Mutation, BaseSeed == 0 ? 0 : BaseSeed + Index));
    return Results;
}
