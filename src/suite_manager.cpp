//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : suite_manager.cpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "tdog/private/suite_manager.hpp"
#include "util.hpp"

//---------------------------------------------------------------------------
// NON-CLASS
//---------------------------------------------------------------------------
using namespace tdog;

//---------------------------------------------------------------------------
// CLASS suite_manager : PRIVATE MEMBERS
//---------------------------------------------------------------------------
int suite_manager::g_counter = 0;
int suite_manager::g_auto_mark = 0;
std::vector<std::string> suite_manager::g_namespace;
//---------------------------------------------------------------------------
// CLASS suite_manager : PUBLIC MEMBERS
//---------------------------------------------------------------------------
suite_manager::suite_manager(bool open, const std::string& suite, bool auto_reg)
{
  // Add or remove from list
  if (open)
  {
    ++g_counter;

    // Mark start explicit suite
    if (!auto_reg && g_auto_mark == 0) g_auto_mark = g_counter;

    // Push name onto namespace sequence
    g_namespace.push_back( trim_str(suite) );
  }
  else
  {
    // Reset auto reg counter
    if (g_auto_mark == g_counter || g_counter <= 0) g_auto_mark = 0;

    --g_counter;

    // Step back namespace sequence
    if (g_namespace.size() != 0) g_namespace.pop_back();
  }
}
//---------------------------------------------------------------------------
std::string suite_manager::current()
{
  // Static method to build current namespace
  std::string rslt;
  for(std::size_t n = 0; n < g_namespace.size(); ++n)
  {
    if (n != 0) rslt += NSSEP;
    rslt += g_namespace[n];
  }

  return rslt;
}
//---------------------------------------------------------------------------
bool suite_manager::auto_reg()
{
  // True if test should self-register with global runner.
  return (g_auto_mark == 0);
}
//---------------------------------------------------------------------------
int suite_manager::counter()
{
  // Static access to counter. A positive value
  // indicates suite without closure, while negative
  // indicates additional closure.
  return g_counter;
}
//---------------------------------------------------------------------------
