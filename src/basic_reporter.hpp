//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : basic_reporter.hpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_BASIC_REPORTER_H
#define TDOG_BASIC_REPORTER_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "tdog/private/basic_test.hpp"

#include <string>
#include <ostream>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

// Contants as defines for portability
#define TDOG_WEBSITE_NAME "kuiper.zone"
#define TDOG_WEBSITE_URL "http://kuiper.zone/"

// Namespace
namespace tdog {

// Forward declarations
class runner;

//---------------------------------------------------------------------------
// CLASS basic_reporter
//---------------------------------------------------------------------------
class basic_reporter
{
  private:

  int m_indent;
  int m_indent_step;
  bool m_verbose;
  const runner* m_runner_ptr;

  void _impl_clear();

  protected:

  // Indentation utility method
  std::string _indent(int step = 0);

  public:

  // Construction
  basic_reporter(const runner* owner, bool verbose);
  virtual ~basic_reporter();

  // Access owner
  virtual const runner& owner() const;

  // Common properties
  bool verbose() const;
  void set_verbose(bool v);
  int indent_step() const;
  void set_indent_step(int step);

  // Report generator methods
  // Must be defined in concrete classes
  virtual void clear();
  virtual void gen_start(std::ostream& os) = 0;
  virtual void gen_test(std::ostream& os, const basic_test* tc) = 0;
  virtual void gen_end(std::ostream& os) = 0;

  // Static string utilities
  static std::string test_type_str(test_type_t t);
  static std::string event_str(event_type_t t);
  static std::string status_str(status_t t);
};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------

