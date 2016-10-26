//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : suite_manager.hpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_SUITE_MANAGER_H
#define TDOG_SUITE_MANAGER_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include <string>
#include <vector>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Keeps tab on the current suite namespace. Relies on the fact that objects
// are constructed in the order they declared in the same compilation unit.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
namespace tdog {

// Namespace separator. Can be
// changed, but there's no need to.
const char* const NSSEP = "::";

//---------------------------------------------------------------------------
// CLASS suite_manager
//---------------------------------------------------------------------------
class suite_manager
{
  private:
  static int g_counter;
  static int g_auto_mark;
  static std::vector<std::string> g_namespace;

  public:

  suite_manager(bool open, const std::string& suite = std::string(),
    bool auto_reg = true);

  static std::string current();
  static bool auto_reg();
  static int counter();
};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
