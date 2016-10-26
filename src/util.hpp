//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : util.hpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_UTIL_H
#define TDOG_UTIL_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "tdog/compat.hpp"

#include <string>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

// Namespace
namespace tdog {

//---------------------------------------------------------------------------
// ROUTINES
//---------------------------------------------------------------------------
std::string ascii_lower(std::string s);
std::string trim_str(std::string s);
std::string int_to_str(tdog::xint_t i, const std::string& format_str = "%d");
std::string double_to_str(double i, const std::string& format_str = "%f");
std::string str_replace(std::string src, const std::string& find, const std::string& rep);
std::string wide_to_str(const std::wstring& s);
std::string wide_to_str(const std::string& s);
std::string encode_esc(const std::string& s);
std::string xml_esc(std::string s);
bool is_suffix(const std::string& s, const std::string& sfx);
std::string hostname();
tdog::i64_t msec_time();
void msleep(int m);
std::string iso_time(tdog::i64_t, bool utc = false);
std::string duration_str(tdog::i64_t t);

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------

