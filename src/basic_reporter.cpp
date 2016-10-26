//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : basci_reporter.cpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "basic_reporter.hpp"
#include "tdog/runner.hpp"

#include <stdexcept>

//---------------------------------------------------------------------------
// NON-CLASS
//---------------------------------------------------------------------------
using namespace tdog;

//---------------------------------------------------------------------------
// CLASS basic_reporter : PRIVATE MEMBERS
//---------------------------------------------------------------------------
void basic_reporter::_impl_clear()
{
  // Internal clear as public clear is virtual.
  m_indent = 0;
}
//---------------------------------------------------------------------------
// CLASS basic_reporter : PROTECTED MEMBERS
//---------------------------------------------------------------------------
std::string basic_reporter::_indent(int step)
{
  // Return indentation. Increments or decrements if step
  // is positive or negative respectively. Note that the
  // increasing step is done after the output is generated.
  std::string rslt;

  if (step < 0) m_indent += (m_indent_step * step);

  if (m_indent > 0) rslt = std::string(m_indent, ' ');

  if (step > 0) m_indent += (m_indent_step * step);

  return rslt;
}
//---------------------------------------------------------------------------
// CLASS basic_reporter : PUBLIC MEMBERS
//---------------------------------------------------------------------------
basic_reporter::basic_reporter(const runner* owner, bool verbose)
{
  // Constructor
  _impl_clear();
  m_verbose = verbose;
  m_indent_step = 2;

  if (owner != 0)
  {
    // Assign explicit owner
    m_runner_ptr = owner;
  }
  else
  {
    // Never allow to happen
    throw std::logic_error("Runner owner is null");
  }
}
//---------------------------------------------------------------------------
basic_reporter::~basic_reporter()
{
  // Destructor
}
//---------------------------------------------------------------------------
void basic_reporter::clear()
{
  // Should be overriden, but call this parent.
  _impl_clear();
}
//---------------------------------------------------------------------------
bool basic_reporter::verbose() const
{
  // Accessor
  return m_verbose;
}
//---------------------------------------------------------------------------
void basic_reporter::set_verbose(bool v)
{
  // Mutator
  m_verbose = v;
}
//---------------------------------------------------------------------------
int basic_reporter::indent_step() const
{
  // Accessor
  return m_indent_step;
}
//---------------------------------------------------------------------------
void basic_reporter::set_indent_step(int step)
{
  // Mutator
  if (step < 0) step = 0;
  m_indent_step = step;
}
//---------------------------------------------------------------------------
const runner& basic_reporter::owner() const
{
  // Runner owner (cannot be null pointer)
  return *m_runner_ptr;
}
//---------------------------------------------------------------------------
std::string basic_reporter::test_type_str(test_type_t t)
{
  // Static test type string
  switch(t)
  {
    case TT_TEST: return "TEST";
    case TT_REPEATED: return "REPEATED";
    case TT_FIXTURE: return "FIXTURE";
    case TT_PROTECTED: return "PROTECTED";
    default: return std::string();
  }
}
//---------------------------------------------------------------------------
std::string basic_reporter::event_str(event_type_t t)
{
  // Static event type string
  switch(t)
  {
    case ET_PASS : return "OK";
    case ET_FAIL : return "FAIL";
    case ET_ERROR : return "ERROR";
    case ET_WARN : return "WARNING";
    default: return std::string();
  }

}
//---------------------------------------------------------------------------
std::string basic_reporter::status_str(status_t t)
{
  // Static status string
  switch(t)
  {
    case TS_READY:        return "NOT RUN";
    case TS_PASS_OK:      return "PASS (OK)";
    case TS_PASS_WARN:    return "PASS (WARN)";
    case TS_FAILED:       return "FAILED";
    case TS_ERROR:        return "ERROR";
    case TS_SKIPPED:      return "SKIPPED";
    case TS_DISABLED:     return "DISABLED";
    default:              return std::string();
  }
}
//---------------------------------------------------------------------------
