//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : run_container.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_RUN_CONTAINER_H
#define TDOG_RUN_CONTAINER_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "run_helper.hpp"
#include "tdog/runner.hpp"

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

// Class wraps user written test code. It should have few methods.

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------
namespace tdog {

//---------------------------------------------------------------------------
// CLASS run_interface
//---------------------------------------------------------------------------
class run_interface
{
  public:

  // Provides runtime test case
  // assert and print functionality.
  run_helper* tdog_helper;

  // Constructor
  run_interface(run_helper* ptr = 0)
    : tdog_helper(ptr) {}

  // The run method. To be
  // defined by TDOG_TEST_CASE().
  virtual void tdog_impl() = 0;
};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
