#include "BattleEquipmentManagerComponent.h"

#include "AbilitySystemGlobals.h"
#include "BattleEquipmentDefinition.h"
#include "BattleEquipmentInstance.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleEquipmentManagerComponent)

UBattleEquipmentInstance* FBattleEquipmentList::AddEntry(TSubclassOf<UBattleEquipmentDefinition> EquipmentDefinition)
{
	UBattleEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	const UBattleEquipmentDefinition* EquipmentCDO = GetDefault<UBattleEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UBattleEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (!InstanceType)
	{
		InstanceType = UBattleEquipmentInstance::StaticClass();
	}

	FBattleAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UBattleEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);
	Result = NewEntry.Instance;

	UBattleAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	check(ASC);
	{
		for (TObjectPtr<UBattleAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, &NewEntry.GrantedHandles, Result);
		}
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);
	
	return Result;
	
}

void FBattleEquipmentList::RemoveEntry(UBattleEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator();EntryIt;++EntryIt)
	{
		FBattleAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			UBattleAbilitySystemComponent* ASC = GetAbilitySystemComponent();
			check(ASC)
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}
			
			Instance->DestroyEquipmentActors();
			EntryIt.RemoveCurrent();
		}
		
	}
}

UBattleAbilitySystemComponent* FBattleEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();

	return Cast<UBattleAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
	
}

UBattleEquipmentManagerComponent::UBattleEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
}

UBattleEquipmentInstance* UBattleEquipmentManagerComponent::EquipItem(
	TSubclassOf<UBattleEquipmentDefinition> EquipmentDefinition)
{
	UBattleEquipmentInstance* Result = nullptr;
	if (EquipmentDefinition)
	{
		Result = EquipmentList.AddEntry(EquipmentDefinition);
		if (Result)
		{
			Result->OnEquipped();
		}
	}

	return Result;	
}

void UBattleEquipmentManagerComponent::UnequipItem(UBattleEquipmentInstance* ItemInstance)
{
	if (ItemInstance)
	{
		ItemInstance->OnUnequipped();

		EquipmentList.RemoveEntry(ItemInstance);		
	}
}

TArray<UBattleEquipmentInstance*> UBattleEquipmentManagerComponent::GetEquipmentInstancesOfType(
	TSubclassOf<UBattleEquipmentInstance> InstanceType) const
{
	TArray<UBattleEquipmentInstance*> Results;

	for (const FBattleAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UBattleEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}

UBattleEquipmentInstance* UBattleEquipmentManagerComponent::GetFirstInstanceOfType(
	TSubclassOf<UBattleEquipmentInstance> InstanceType)
{
	for (FBattleAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UBattleEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}
	return nullptr;
}
