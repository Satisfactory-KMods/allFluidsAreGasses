// Fill out your copyright notice in the Description page of Project Settings.


#include "AFAGWorldModule.h"

#include "Buildables/FGBuildableGeneratorFuel.h"
#include "Buildables/FGBuildableResourceExtractorBase.h"
#include "Subsystems/KBFLAssetDataSubsystem.h"
#include "Subsystems/KBFLContentCDOHelperSubsystem.h"

DECLARE_LOG_CATEGORY_EXTERN( AFAGWorldModuleLog, Log, All );

DEFINE_LOG_CATEGORY( AFAGWorldModuleLog );

UAFAGWorldModule::UAFAGWorldModule()
{
	bRootModule = true;
}

void UAFAGWorldModule::ConstructionPhase_Delayed()
{
	Super::ConstructionPhase_Delayed();

	const UKBFLContentCDOHelperSubsystem* CDOHelperSubsystem = UKBFLContentCDOHelperSubsystem::Get( GetWorld() );
	const UKBFLAssetDataSubsystem* AssetDataSubsystem = UKBFLAssetDataSubsystem::Get( GetWorld() );

	if( CDOHelperSubsystem && AssetDataSubsystem )
	{
		OverwriteItems();
		return;
	}
	GetWorld()->GetTimerManager().SetTimerForNextTick( this, &UKBFLGameInstanceModule::ConstructionPhase_Delayed );
}

void UAFAGWorldModule::OverwriteItems()
{
	UKBFLContentCDOHelperSubsystem* CDOHelperSubsystem = UKBFLContentCDOHelperSubsystem::Get( GetWorld() );
	UKBFLAssetDataSubsystem* AssetDataSubsystem = UKBFLAssetDataSubsystem::Get( GetWorld() );

	if( AssetDataSubsystem && CDOHelperSubsystem )
	{
		TArray< TSubclassOf< UFGItemDescriptor > > Items;
		AssetDataSubsystem->GetItemsOfForms( { EResourceForm::RF_LIQUID, EResourceForm::RF_GAS }, Items );
		if( Items.Num() > 0 )
		{
			for( TSubclassOf< UFGItemDescriptor > Item : Items )
			{
				if( UFGItemDescriptor* Default = CDOHelperSubsystem->GetAndStoreDefaultObject_Native< UFGItemDescriptor >( Item ) )
				{
					Default->mForm = EResourceForm::RF_GAS;
					Default->MarkPackageDirty();
					UE_LOG( AFAGWorldModuleLog, Log, TEXT("Modify Item %s to Gas"), *UFGItemDescriptor::GetItemName(Item).ToString() );
				}
			}
		}

		TArray< TSubclassOf< AFGBuildable > > Buildings;
		AssetDataSubsystem->GetBuildableOfChilds( { AFGBuildableResourceExtractorBase::StaticClass(), AFGBuildableGeneratorFuel::StaticClass(), }, Buildings );

		if( Buildings.Num() > 0 )
		{
			for( TSubclassOf< AFGBuildable > Building : Buildings )
			{
				if( Building->IsChildOf( AFGBuildableResourceExtractorBase::StaticClass() ) )
				{
					AFGBuildableResourceExtractorBase* DefaultObject = CDOHelperSubsystem->GetAndStoreDefaultObject_Native< AFGBuildableResourceExtractorBase >( Building );
					if( DefaultObject )
					{
						DefaultObject->mAllowedResourceForms.AddUnique( EResourceForm::RF_GAS );
						DefaultObject->MarkPackageDirty();
						UE_LOG( AFAGWorldModuleLog, Log, TEXT("Add Gas as valid for building: %s"), *DefaultObject->GetName() );
					}
				}

				else if( Building->IsChildOf( AFGBuildableGeneratorFuel::StaticClass() ) )
				{
					AFGBuildableGeneratorFuel* DefaultObject = CDOHelperSubsystem->GetAndStoreDefaultObject_Native< AFGBuildableGeneratorFuel >( Building );
					if( DefaultObject )
					{
						DefaultObject->mFuelResourceForm = EResourceForm::RF_GAS;
						DefaultObject->MarkPackageDirty();
						UE_LOG( AFAGWorldModuleLog, Log, TEXT("Set Gas as valid for building: %s"), *DefaultObject->GetName() );
					}
				}
			}
		}
	}
}
