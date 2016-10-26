//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : text_reporter.hpp
// COPYRIGHT    : Kuiper (c) 2016
// WEBSITE      : kuiper.zone
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_TEXT_REPORTER_H
#define TDOG_TEXT_REPORTER_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "basic_reporter.hpp"

#include <string>
#include <vector>
#include <ostream>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

// Namespace
namespace tdog {

//---------------------------------------------------------------------------
// CLASS text_reporter
//---------------------------------------------------------------------------
class text_reporter : public basic_reporter
{
  private:

  bool m_decl_flag;
  int m_break_text_width;
  std::string m_current_suite;
  std::vector<std::string> m_skipped_tests;

  // For use by html_reporter
  bool m_testname_leader;
  bool m_suite_breaks;

  void _impl_clear();
  void _write_txt_break(std::ostream& os) const;

  public:

  // Constructor
  text_reporter(const runner* owner, bool verbose = true);

  // Properties
  int break_text_width() const;
  void set_break_text_width(int bw);
  bool testname_leader() const;
  void set_testname_leader(bool tn);
  bool suite_breaks() const;
  void set_suite_breaks(bool sb);

  // Report generator methods
  virtual void clear();
  virtual void gen_start(std::ostream& os);
  virtual void gen_test(std::ostream& os, const basic_test* tc);
  virtual void gen_end(std::ostream& os);
};

} // namespace

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------

