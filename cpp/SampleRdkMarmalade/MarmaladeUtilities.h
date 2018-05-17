
#pragma once

_locale_t Locale(void);

inline double StringToFloat(const TCHAR* s) { return _tstof_l(s, Locale()); }
