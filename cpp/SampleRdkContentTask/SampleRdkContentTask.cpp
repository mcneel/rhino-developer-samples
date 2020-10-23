
#include "stdafx.h"
#include "SampleRdkContentTask.h"
#include "SampleRdkContentTaskPlugIn.h"

// This task is registered as an RDK extension. See CSampleRdkContentTaskRdkPlugIn::RegisterExtensions().

UUID CContentTask::PlugInId(void) const
{
	return SampleRdkContentTaskPlugIn().PlugInID();
}

int CContentTask::MenuOrder(const IRhRdkTaskOrigin& origin) const
{
	// TODO: Set menu sort order to specify the task's position in the menu. Must be greater than zero.
	// Values between 100 and 10000 are best. Please make sure your task does not appear inside an unrelated
	// group of existing menu items.
	return 5000;
}

const wchar_t* CContentTask::MenuString(const IRhRdkTaskOrigin& origin, CRhRdkContent::Kinds kind) const
{
	return L"TODO: Your task menu string here";
}

CRhRdkTask::Result CContentTask::Execute(const IRhRdkTaskOrigin& origin) const
{
	// TODO: Execute your task here.

	return Result::Success;
}
