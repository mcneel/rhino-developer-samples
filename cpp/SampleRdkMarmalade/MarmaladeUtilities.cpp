
#include "stdafx.h"
#include "MarmaladeUtilities.h"

_locale_t Locale(void)
{
	static _locale_t loc = NULL;
	if (NULL == loc)
		loc = _create_locale(LC_ALL, "C");
	return loc;
}
