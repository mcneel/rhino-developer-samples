import rhinoscriptsyntax as rs
import System

locale_id = rs.LocaleID()
culture = System.Globalization.CultureInfo(locale_id)
print culture.EnglishName
