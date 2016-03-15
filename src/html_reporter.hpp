//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : html_reporter.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER GUARD
//---------------------------------------------------------------------------
#ifndef TDOG_HTML_REPORTER_H
#define TDOG_HTML_REPORTER_H

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

// Forward declarations
class text_reporter;

//---------------------------------------------------------------------------
// CLASS html_reporter
//---------------------------------------------------------------------------
class html_reporter : public basic_reporter
{
  private:

  bool m_decl_flag;
  bool m_suite_added;
  std::string m_current_suite;
  text_reporter* m_text_reporter_ptr;
  std::vector<std::string> m_pass_trace;
  std::vector<std::string> m_fail_trace;
  std::vector<std::string> m_error_trace;
  std::vector<std::string> m_skipped_tests;

  void _impl_clear();
  std::ostream& _generate_css(std::ostream& os);
  static std::string _clean_id(const std::string& id_str);
  static std::string _page_link(const std::string& id_str,
    const std::string& name_str);
  static std::string _page_anchor(const std::string& id_str);
  static std::string _th_str(const std::string& content,
    const std::string& width = std::string());
  static std::string _td_str(const std::string& content, bool bold = false,
    const std::string& class_id = std::string());
  static std::string _cond_classid(status_t s = TS_READY);
  void _build_trace(std::ostream& os, const std::vector<std::string>& trace_vec,
    const std::string& title_elem);

  public:

  // Constructor
  html_reporter(const runner* owner, bool verbose = true);
  ~html_reporter();

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

