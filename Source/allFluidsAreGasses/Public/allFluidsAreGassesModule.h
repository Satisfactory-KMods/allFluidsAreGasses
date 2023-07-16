#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

class FallFluidsAreGassesModule: public IModuleInterface {
	public:
		/** IModuleInterface implementation */
		virtual void StartupModule() override;
};
