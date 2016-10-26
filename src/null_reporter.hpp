//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : null_reporter.hpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_NULL_REPORTER_H
#define TDOG_NULL_REPORTER_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "basic_reporter.hpp"

#include <string>
#include <ostream>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

// Namespace
namespace tdog {

// Forward declarations
class runner;
class basic_test;

//---------------------------------------------------------------------------
// CLASS null_reporter
//---------------------------------------------------------------------------
class null_reporter : public basic_reporter
{
  public:

  // Constructor
  null_reporter(const runner* owner, bool verbose = true)
    : basic_reporter(owner, verbose) {}

  // Report generator methods
  virtual void gen_start(std::ostream& os);
  virtual void gen_test(std::ostream& os, const basic_test* tc);
  virtual void gen_end(std::ostream& os);
};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------

