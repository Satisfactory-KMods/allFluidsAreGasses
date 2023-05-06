#include "allFluidsAreGassesModule.h"

#include "Buildables/FGBuildableGenerator.h"
#include "Buildables/FGBuildableGeneratorFuel.h"
#include "Buildables/FGBuildableResourceExtractorBase.h"
#include "Patching/NativeHookManager.h"

DECLARE_LOG_CATEGORY_EXTERN( AFAGHookLog, Log, All );

DEFINE_LOG_CATEGORY( AFAGHookLog );

void FallFluidsAreGassesModule::StartupModule()
{
#if !WITH_EDITOR
	SUBSCRIBE_METHOD_VIRTUAL( AFGBuildableGeneratorFuel::BeginPlay, GetMutableDefault<AFGBuildableGeneratorFuel>(), [](CallScope<void(*)(AFGBuildableGeneratorFuel*)>& scope, AFGBuildableGeneratorFuel* selfref) { if(selfref->mFuelResourceForm == EResourceForm::RF_LIQUID) { selfref->mFuelResourceForm = EResourceForm::RF_GAS; UE_LOG(AFAGHookLog, Log, TEXT("make Gas Valid: %s"), *selfref->GetName()); } } );

	SUBSCRIBE_METHOD_VIRTUAL( AFGBuildableResourceExtractorBase::BeginPlay, GetMutableDefault<AFGBuildableResourceExtractorBase>(), [](CallScope<void(*)(AFGBuildableResourceExtractorBase*)>& scope, AFGBuildableResourceExtractorBase* selfref) { selfref->mAllowedResourceForms.AddUnique(EResourceForm::RF_GAS); UE_LOG(AFAGHookLog, Log, TEXT("make Gas Valid: %s"), *selfref->GetName()); } );
#endif
}

IMPLEMENT_GAME_MODULE( FallFluidsAreGassesModule, allFluidsAreGasses );
