#pragma once

#include "IDetailCustomization.h"

class FRosConnectionToolCustomization : public IDetailCustomization
{
public:
	//IDetailCustomization interface
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	//End of IdetailCustomization itnerface

	static TSharedRef<IDetailCustomization> MakeInstance();

	static FReply ExecuteCommand(IDetailLayoutBuilder* DetailBuilder, UFunction* MethodToExecute);

};