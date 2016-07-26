//---------------------------------------------------------------------------
// PROJECT      : TDOG
// FILENAME     : tdog.hpp
// COPYRIGHT    : Andy Thomas (c) 2016
// WEBSITE      : bigangrydog.com
// LICENSE      : Apache 2.0
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// HEADER
//---------------------------------------------------------------------------
#ifndef TDOG_H
#define TDOG_H

//---------------------------------------------------------------------------
// INCLUDES
//---------------------------------------------------------------------------
#include "tdog/types.hpp"
#include "tdog/runner.hpp"
#include "tdog/private/run_helper.hpp"
#include "tdog/private/basic_test.hpp"
#include "tdog/private/test_case.hpp"
#include "tdog/private/test_fixture.hpp"
#include "tdog/private/run_interface.hpp"
#include "tdog/private/suite_manager.hpp"

#include <string>

//---------------------------------------------------------------------------
// DOCUMENTATION
//---------------------------------------------------------------------------
/**
 * \file
 * \brief This is the include file for the TDOG unit testing framework. It is the
 * only one you need.
 */

//---------------------------------------------------------------------------
// DECLARATIONS
//---------------------------------------------------------------------------

#if DOXYGEN
// Commenting this out will cause macro implementations
// to be included in the DOXYGEN output.
#define DOXYGEN_HIDEIMPL
#endif

// Used in implementation only
#define TDOG_CONCAT_IMPL( x, y ) x##y
#define TDOG_CONCAT( x, y ) TDOG_CONCAT_IMPL( x, y )

//---------------------------------------------------------------------------
// TEST MACROS
//---------------------------------------------------------------------------

/**
 * @name Library version macros
 * @{
 */

/**
 * \brief Library name string constant.
 */
#define TDOG_LIB_NAME "TDOG C++ xUnit Testing Framework"

/**
 * \brief Library version number.
 */
#define TDOG_LIB_VERSION "3.0"

/**
 * @}
 * @name Test declaration macros
 * @{
 */

/**
 * \brief Declares and implements a test case.
 * \details A test can be created as follows:
 *
 * \code
 * TDOG_TEST_CASE(number_test)
 * {
 *   int num_beast = 665;
 *   TDOG_ASSERT_EQ(666, ++num_beast); // Passes
 * }
 * \endcode
 *
 * The test name should not contain spaces, as it will be used internally to
 * form the basis of a C++ data type. The test code should not return any result.
 *
 * Tests may be declared inside a suite using TDOG_SUITE(), or in the global
 * suite namespace. Such tests will automatically "self-register" with a global
 * instance of the tdog::runner() class, so that may be run simply by calling the
 * TDOG_RUN() macro. To create a test that does not "self-register", see
 * TDOG_EXPLICIT_SUITE().
 *
 * Typically, test cases would be defined in header files to be included in
 * the file containing the main() function.
 * \param[in] test_name The test name
 * \sa TDOG_SUITE(), TDOG_EXPLICIT_SUITE(), TDOG_TEST_PROTECTED(),
 * TDOG_DEFINE_REPEATED(), TDOG_TEST_FIXTURE(), TDOG_RUN()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Dummy doc
  #define TDOG_TEST_CASE(test_name)
#else
  #define TDOG_TEST_CASE(test_name) \
  class test_name##_testclass : public tdog::run_interface { \
  public: \
    virtual void tdog_impl(); \
    test_name##_testclass(tdog::run_helper* ptr) \
      : tdog::run_interface(ptr) {} \
  }; \
  tdog::test_case<test_name##_testclass> test_name##_inst( \
    #test_name, tdog::TT_TEST, __FILE__, __LINE__); \
  void test_name##_testclass::tdog_impl()
#endif

/**
 * \brief Defines a test that can be repeated using a supplied data type.
 * \details This macro defines a test implementation, but does not instantiate it.
 * It can be used to create a test that can be used multiple times with a
 * different data type.
 *
 * Let's assume, for a moment, that we have written several pseudo random number
 * generation classes, each derived from the same base class which has an "interface"
 * which looks like this:
 *
 * \code
    class rand_obj
    {
    public:
      rand_obj(int seed); // <- non default constructor
      virtual int get_random() = 0; // <- pure virtual
    };
 * \endcode
 *
 * What we would not want to do is write the same test code, multiple times,
 * in order to test each of our concrete random generator classes. Ideally,
 * we would want to define a single test implementation, and "instantiate"
 * the same test with all concrete types to test them the same way.
 *
 * Here's how to do it:
 *
 * \code
 * // Define repeated test
 * TDOG_DEFINE_REPEATED(rand_test)
 * {
 *   // Create instance of random generator
 *   USER_TYPE rand(237823); // <- fixed seed
 *
 *   // Test result is different on the second call.
 *   // Not a realistic test of randomness, but just an example.
 *   int first = rand.get_random();
 *   TDOG_ASSERT_NEQ(first, rand.get_random());
 * }
 *
 * // Instatiate actual tests with concrete generators
 * TDOG_TEST_REPEATED(mt32_test, rand_test, rand_mt32);
 * TDOG_TEST_REPEATED(lcg64_test, rand_test, rand_lcg64);
 * TDOG_TEST_REPEATED(lcg64_test, rand_test, rand_isaac32);
 * \endcode
 *
 * Unlike TDOG_TEST_PROTECTED() or TDOG_TEST_FIXTURE(), the test is not derived
 * from the supplied "user_type", but is implementated internally as template class.
 * In the above, we define the implementation using TDOG_DEFINE_REPEATED(), and give
 * it a repeated  "test typename". The typedef "USER_TYPE" will defined for use in
 * the test implementation, as shown above. The test is not instantiated, however, until
 * we declare TDOG_TEST_REPEATED(), which we must supply with a unique test name
 * (i.e. "mt32_test"), the repeated test type ("rand_test"), and the concrete
 * "user_type" with which to test.
 *
 * Moreover, a repeated test can be used to fulfil a different test requirement.
 *
 * Here, we will assume that we have written a function overloaded for both
 * std::string std::wstring types, which looks like this:
 *
 * \code
 * // Return true if strings are same case insensitively
 * bool insensitive_compare(const std::string& s1, const std::string& s1);
 * bool insensitive_compare(const std::wstring& s1, const std::wstring& s1);
 * \code
 *
 * We could test both of these as follows:
 *
 * \code
 * TDOG_DEFINE_REPEATED(compare_test)
 * {
 *   // Initialise
 *   USER_TYPE upper = USER_TYPE(3, 'A'); // <- see basic_string constructor
 *   USER_TYPE lower = USER_TYPE(3, 'a');
 *
 *   // Positive test
 *   TDOG_ASSERT( insensitive_compare(upper, lower) );
 *
 *   // Negative test
 *   USER_TYPE different = USER_TYPE(3, 'b');
 *   TDOG_ASSERT_NOT( insensitive_compare(upper, different) );
 * }
 *
 * // Now instantiate with both string types
 * TDOG_TEST_REPEATED(narrow_test, compare_test, std::string);
 * TDOG_TEST_REPEATED(wide_test, compare_test, std::wstring);
 * \endcode
 *
 * When executed, the test names shown in the reports will be "narrow_test"
 * and "wide_type" respectively, but file and line number information will
 * refer to where the implementation is defined. Use the fully qualified
 * "test names" when running or disabling test, rather than the repeated
 * test typename.
 * \param[in] repeat_type Typename for the test implementation
 * \sa TDOG_TEST_REPEATED(), TDOG_TEST_PROTECTED(), TDOG_TEST_FIXTURE(), TDOG_TEST_CASE()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_DEFINE_REPEATED(repeat_type)
#else
  #define TDOG_DEFINE_REPEATED(repeat_type) \
  template <typename T> \
  class repeat_type##_reptclass : public tdog::run_interface { \
  public: \
    typedef T USER_TYPE; \
    virtual void tdog_impl(); \
    repeat_type##_reptclass(tdog::run_helper* ptr) \
      : tdog::run_interface(ptr) {} \
  }; \
  template <typename T> void repeat_type##_reptclass<T>::tdog_impl()
#endif

/**
 * \brief Instantiates a repeated test implementation with the supplied
 * user data type.
 * \details See TDOG_DEFINE_REPEATED() for explanation and examples.
 * \param[in] test_name Name of the test
 * \param[in] repeat_type Typename for the test implementation
 * \param[in] user_type Typename of the thing we want to test
 * \sa TDOG_DEFINE_REPEATED()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_TEST_REPEATED(test_name, repeat_type, user_type)
#else
  #define TDOG_TEST_REPEATED(test_name, repeat_type, user_type) \
  tdog::test_case< repeat_type##_reptclass<user_type > > test_name##_inst( \
    #test_name, tdog::TT_REPEATED, __FILE__, __LINE__, #user_type, #repeat_type);
#endif

/**
 * \brief Declares and implements a unit test which has access to the
 * protected members of the type provided.
 * \details In TDOG_TEST_PROTECTED(), the test case is derived from the
 * class or struct type provided, thus allowing the test code access to
 * its protected members. The supplied type must have a default constructor,
 * and be instantiatable.
 *
 * This test type is similar to TDOG_TEST_FIXTURE(), except that setup()
 * and teardown() methods are not called (the supplied type need not
 * define them). Additionally, the typedef "USER_TYPE" will be defined
 * for use in the test implementation, so that additional instances of
 * the type maybe instantiated as required.
 *
 * Example:
 *
 * class test_obj
 * {
 * protected:
 *   bool protected_routine() const;
 * public:
 *   bool public_routine() const;
 * };
 *
 * ...
 *
 * TDOG_TEST_PROTECTED(test_the_object, test_obj)
 * {
 *   TDOG_ASSERT( protected_routine() );
 *
 *   // New instance test
 *   USER_TYPE obj;
 *   TDOG_ASSERT( obj.public_routine() );
 * }
 * \endcode
 * \param[in] test_name Name of the test
 * \param[in] user_type Typename of the thing we want to test
 * \sa TDOG_TEST_REPEATED(), TDOG_TEST_PROTECTED(), TDOG_TEST_FIXTURE(), TDOG_TEST_CASE()
*/
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Dummy doc
  #define TDOG_TEST_PROTECTED(test_name, user_type)
#else
  #define TDOG_TEST_PROTECTED(test_name, user_type) \
  class test_name##_protclass : public tdog::run_interface, public user_type { \
  public: \
    typedef user_type USER_TYPE; \
    virtual void tdog_impl(); \
    test_name##_protclass(tdog::run_helper* ptr) \
      : tdog::run_interface(ptr), user_type() {} \
  }; \
  tdog::test_case<test_name##_protclass> test_name##_inst( \
    #test_name, tdog::TT_PROTECTED, __FILE__, __LINE__, #user_type); \
  void test_name##_protclass::tdog_impl()
#endif

/**
 * \brief Creates a test with custom data and setup/teardown functionality.
 * \details In TDOG_TEST_FIXTURE(), the test case is derived from the
 * class or struct type provided, and is specifically intended to provide
 * test level setup() and teardown() routines.
 *
 * The supplied "user type" must implement the following methods:
 *
 * \code
 *   bool setup();
 *   void teardown();
 * \endcode
 *
 * Additionally, it must have a default constructor, and be instantiatable.
 *
 * When the test is executed, the setup() method will be called first. It
 * should return true on success, otherwise neither the test itself or the
 * the teardown() routine will be called. If setup() returns false, or either
 * setup() or teardown() throw an exception, the test will be set to the "error"
 * state. You may also choose to put setup and teardown functionality in the
 * constructor and destructor, but note that if the destructor throws an
 * exception, behaviour is undefined.
 *
 * Like TDOG_PROTECTED_TEST(), the unit test code will have access to the
 * supplied type's protected members, although no USER_TYPE is defined
 * for this test.
 *
 * Example
 *
 * \code
 * // Declare fixture type
 * struct person_t
 * {
 *   std::string name;
 *   int age;
 *
 *   bool setup()
 *   {
 *     name = "unknown";
 *     age = 25;
 *     return true; // <- success
 *   }
 *
 *   void teardown()
 *   {
 *     // Any teardown code
 *   }
 * };
 *
 * // Create test fixture
 * TDOG_TEST_FIXTURE(test_person, person_t)
 * {
 *   TDOG_ASSERT_EQ_MSG("unknown", name, "Person name test");
 *   TDOG_ASSERT_EQ_MSG(25, age, "Default age");
 * }
 * \endcode
 *
 * Global and suite level setup and teardown routines may also
 * be created with "test cases" with name case insensitive names
 * beginning with "setup" and "teardown" respectively. See the
 * \link maindoc.hpp Main page \endlink for information.
 * \param[in] test_name The test name
 * \param[in] user_type The base test type which to inherit
 * \sa TDOG_DEFINE_REPEATED(), TDOG_TEST_CASE(), TDOG_SUITE()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // This one drove me mental!
  #define TDOG_TEST_FIXTURE(test_name, user_type)
#else
  #define TDOG_TEST_FIXTURE(test_name, user_type) \
  class test_name##_fixtclass : public tdog::run_interface, public user_type { \
  public: \
    virtual void tdog_impl(); \
    test_name##_fixtclass(tdog::run_helper* ptr) \
      : tdog::run_interface(ptr), user_type() {} \
  }; \
  tdog::test_fixture<test_name##_fixtclass> test_name##_inst( \
    #test_name, tdog::TT_FIXTURE, __FILE__, __LINE__, #user_type); \
  void test_name##_fixtclass::tdog_impl()
#endif

/**
 * \brief Allows "auto-registering" tests to be namespaced and grouped into
 * "suites".
 * \details A test suite allows test cases to be run or disabled in groups.
 * The suite name will be used directly as a C++ namespace around its test cases,
 * so it must not contain spaces. Declaring tests within namespaced suites
 * also allows the same test name to be used in multiple contexts.
 *
 * Tests within the suite will automatically "self-register" with a global
 * instance of the tdog::runner class. This means that they can simply be
 * defined and run using the TDOG_RUN() macro, without having to deal with
 * a runner class directly. However, if you need the flexibility to register
 * tests explicity with your own code, the macro TDOG_EXPLICIT_SUITE() is
 * provided to allow you to do this.
 *
 * When declaring a test suite, it is important to "close" the suite with a
 * TDOG_CLOSE_SUITE statement at the end of namespace scope. This is
 * needed to allow the runner to properly track where one suite namespace
 * ends, and a new one begins.
 *
 * In the following example, a unit test is defined inside a test suite:
 *
 * \code
 * TDOG_SUITE(beast_suite)
 * {
 *   TDOG_TEST_CASE(number_test)
 *   {
 *     int num_beast = 666;
 *     TDOG_ASSERT_EQ(666, num_beast); // Passes
 *   }
 *
 *   // Must close test suite
 *   TDOG_CLOSE_SUITE
 * }
 * \endcode
 *
 * Because a suite is effectively a namespace, it may also declare data
 * and functions for use in test implementation. This may be used to
 * provide a mechanism to inject parameters into tests, and retrieve
 * data set by them.
 *
 * Suites may also be nested, and the following demonstrates a number of things:
 *
 * \code
 * namespace project_namespace
 * {
 *   TDOG_SUITE(example_suite)
 *   {
 *     TDOG_SUITE(beast_suite)
 *     {
 *       int INIT_BEAST = 0;
 *
 *       TDOG_TEST_CASE(neighbor_test)
 *       {
 *         int neighbor_of_beast = INIT_BEAST + 2;
 *         TDOG_ASSERT_EQ(668, neighbor_of_beast); // Passes
 *       }
 *
 *       // beast_suite
 *       TDOG_CLOSE_SUITE
 *     }
 *
 *     // example_suite
 *     TDOG_CLOSE_SUITE
 *   }
 * }
 *
 * ...
 *
 * // Set value, and run the neighbor test
 * project_namespace::example_suite::beast_suite::INIT_BEAST = 666; // <- inc. project_namespace
 * TDOG_RUN("example_suite::best_suite::neighbor_test"); // <- exc. project_namespace
 *
 *\endcode
 *
 * Here we have a test case inside a nested suite that uses a data value
 * declared within the suite. We set this to the required value prior
 * to running the test.
 *
 * Furthermore, to demonstrate an important point, we have declared the
 * suites inside the ordinary namespace, "project_namespace".
 * This is allowed. However, to run the test, you must refer to it with
 * the string name "example_suite::beast_suite::number_test", rather than
 * "project_namespace::example_suite::beast_suite::number_test", as TDOG
 * cannot track the namespace name without the use of the TDOG_SUITE()
 * macro.
 *
 * Typically, suites and test cases would be defined in header files
 * to be included in the compilation unit containing the main() function.
 * Test suite names may also be contiguous, i.e. defined across multiple
 * header files, provided the compiler supports the commonly available
 * "__COUNTER__" macro. (See TDOG_COUNTER_ID for more information on this.)
 * \param[in] suite_name
 * \sa TDOG_CLOSE_SUITE, TDOG_EXPLICIT_SUITE(), TDOG_TEST_CASE(),
 * TDOG_COUNTER_ID
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  // The volatile keyword is intended to prevented optimisation here.
  #define TDOG_SUITE(suite_name)
#else
  #define TDOG_SUITE(suite_name) \
  namespace suite_name { \
    namespace TDOG_CONCAT(tdog_suite, TDOG_COUNTER_ID) { \
      volatile const tdog::suite_manager open_suite(true, #suite_name, true); \
    } \
  } \
  namespace suite_name
#endif

/**
 * \brief Declares a suite of tests that do not "self-register" with the
 * global runner instance.
 * \details Tests within normal suites will automatically "self-register"
 * with a global instance of the tdog::runner class. This means that
 * they can simply be defined and run using the TDOG_RUN() macro,
 * without having to deal with a runner class directly.
 *
 * Test cases within a TDOG_EXPLICIT_SUITE(), on the other hand, will
 * not self-register, but must explicitly be registered with an instance
 * of tdog::runner before they can be executed. The TDOG_GET_TCPTR()
 * macro should be used to do this, so as to provide the runner with a
 * pointer to the test case instance.
 *
 * A suite declared with TDOG_EXPLICIT_SUITE() must be closed with a
 * TDOG_CLOSE_SUITE statement, just like normal suites.
 *
 * Example:
 *
 * \code
 * include <tdog.hpp>
 *
 * TDOG_EXPLICIT_SUITE(explicit_suite)
 * {
 *   // Define some test
 *   TDOG_TEST_CASE(some_test)
 *   {
 *     TDOG_ASSERT(true);
 *   }
 *
 *   // Must close the suite
 *   TDOG_CLOSE_SUITE
 * }
 *
 * ...
 *
 * int main(int argc, char **argv)
 * {
 *   // Register the test with a non-global runner
 *   tdog::runner tr;
 *   tr.register_test( TDOG_GET_TCPTR(explicit_suite::some_test) );
 *
 *   // Run the test
 *   return tr.run("*");
 * }
 * \endcode
 *
 *
 * Tests within sub-suites of the TDOG_EXPLICIT_SUITE() will also
 * require explicit registration with a runner, irrespective of
 * whether the sub-suite is declared using TDOG_SUITE() or
 * TDOG_EXPLICIT_SUITE(). Tests in the global suite namespace cannot
 * be made "explicit", and will always self-register.
 *
 * This is an advanced feature which provides for flexibility,
 * if required. It is suggested that you use TDOG_SUITE() in normal
 * circumstances, however.
 * \param[in] suite_name
 * \sa TDOG_CLOSE_SUITE, TDOG_SUITE(), TDOG_TEST_CASE(),
 * tdog::runner, TDOG_COUNTER_ID
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  // The volatile keyword is intended to prevented optimisation here.
  #define TDOG_EXPLICIT_SUITE(suite_name)
#else
  #define TDOG_EXPLICIT_SUITE(suite_name) \
  namespace suite_name { \
    namespace TDOG_CONCAT(tdog_suite, TDOG_COUNTER_ID) { \
      volatile const tdog::suite_manager open_suite(true, #suite_name, false); \
    } \
  } \
  namespace suite_name
#endif

/**
 * \brief Test suite closure macro.
 * \details When declaring a test suite, it is important to "close" the suite
 * scope by declaring TDOG_CLOSE_SUITE after any test case definitions. This is
 * needed to allow the runner to properly track where one suite namespace
 * ends, and a new one begins.
 *
 * Example:
 *
 * \code
 * TDOG_SUITE(suite_name)
 * {
 *  // Test cases
 *  ...
 *
 *  // Must close suite
 *  TDOG_CLOSE_SUITE
 * }
 * \endcode
 *
 * In the event of a missing suite closure statement, the code will compile
 * but the test run will be aborted and the report will display a warning to
 * indicate the problem.
 * \sa TDOG_SUITE()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  // The volatile keyword is intended to prevented optimisation here.
  #define TDOG_CLOSE_SUITE
#else
  #define TDOG_CLOSE_SUITE \
  namespace TDOG_CONCAT(tdog_suite, TDOG_COUNTER_ID) { \
    volatile const tdog::suite_manager close_suite(false); \
  }
#endif

/**
 * @}
 * @name Test property macros
 * @{
 */

/**
 * \brief Allows an author name to be associated with the test case.
 * \details This macro is intended to be applicable in multi-programmer
 * environments, and provides a means of associating an author name with a
 * test. The author name will be displayed in test reports where applicable.
 *
 * It should be used within the test implementation as follows:
 *
 * \code
 * TDOG_TEST_CASE(some_test)
 * {
 *   TDOG_SET_AUTHOR("Employee 24601");
 *   ...
 * }
 * \endcode
 *
 * The underlying input type is std::string.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] author_str Author name (i.e. a std::string or const char*)
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SET_AUTHOR(author_str)
#else
  // Actual
  #define TDOG_SET_AUTHOR(author_str) \
  tdog_helper->set_author(author_str)
#endif

/**
 * \brief Sets a time limit (in milliseconds) for the test execution.
 * \details If the time taken to execute the test exceeds this value, the
 * test will automatically fail.
 *
 * Example:
 *
 * \code
 * TDOG_TEST_CASE(is_prime_func)
 * {
 *   TDOG_SET_TEST_TIMEOUT(5000); // 5 second timeout
 *   TDOG_ASSERT( is_prime(HUGE_PRIME_NUM) );
 * }
 * \endcode
 *
 * A value of 0 (the default) disables the local time limit.
 *
 * If a global time has also been set, the shortest time constraint will apply.
 *
 * Note that test execution will not be interrupted should the time
 * exceed the specified limit (the test will simply enter the failed
 * state, even if all its test conditions pass). However, the
 * TDOG_HAS_TEST_TIMED_OUT() can be used to determine whether the
 * test has exceeded its time constraint and exit appropriately.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] ms Integer time limit in milliseconds
 * \sa TDOG_SET_GLOBAL_LIMIT_EXEMPT(), TDOG_SET_GLOBAL_TIME_WARNING()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SET_TEST_TIMEOUT(ms)
#else
  // Actual
  #define TDOG_SET_TEST_TIMEOUT(ms) \
  tdog_helper->set_time_limit(ms)
#endif

/**
 * \brief Returns true if the time constraint for the test has been exceeded.
 * \details This macro may be called by the test implementation to determine
 * whether the local or global time constraint for the test has been exceeded.
 * This allows the test to determine whether it should cease further
 * testing.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \return Boolean result
 * \sa TDOG_HAS_TEST_FAILED()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_HAS_TEST_TIMED_OUT()
#else
  // Actual
  #define TDOG_HAS_TEST_TIMED_OUT() \
  tdog_helper->check_time_limit(__LINE__)
#endif

/**
 * \brief Designates the test as being exempt from the global
 * time limit setting.
 * \details In addition to setting local time limits for test cases
 * using the TDOG_SET_TEST_TIMEOUT() macro, it is possible to specify a
 * global time limit to be applied to all tests. For certain tests,
 * however, it may be desirable to exempt them from the global time limit.
 *
 * This can be achieved with the TDOG_SET_GLOBAL_LIMIT_EXEMPT() as follows:
 *
 * \code
 * TDOG_TEST_CASE(is_prime_func)
 * {
 *   TDOG_SET_GLOBAL_LIMIT_EXEMPT();
 *   TDOG_ASSERT( is_prime(HUGE_PRIME_NUM) );
 * }
 * \endcode
 *
 * Note also that where a test case defines its own local time limit,
 * the global time limit (if set) will not apply to that test.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \sa TDOG_SET_TEST_TIMEOUT(), tdog::runner::set_global_time_limit()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SET_GLOBAL_LIMIT_EXEMPT()
#else
  // Actual
  #define TDOG_SET_GLOBAL_LIMIT_EXEMPT() \
  tdog_helper->set_global_limit_exempt()
#endif

/**
 * \brief Returns true if the test has failed (i.e. because an assert
 * condition was not met).
 * \details This macro may be called by the test implementation to determine
 * whether the test has failed due to an assert condition not being met.
 * This allows the test to determine whether it should cease further
 * testing.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \return Boolean result
 * \sa TDOG_HAS_TEST_TIMED_OUT()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_HAS_TEST_FAILED()
#else
  // Actual
  #define TDOG_HAS_TEST_FAILED() \
  tdog_helper->has_failed()
#endif

/**
 * @}
 * @name Test Assert Macros
 * @{
 */

/**
 * \brief Asserts that the test condition is true (test fails if
 * it is not).
 * \details The test will fail unless the condition is met.
 *
 * Example:
 *
 * \code
 * TDOG_TEST_CASE(is_prime_func)
 * {
 *   TDOG_SET_GLOBAL_LIMIT_EXEMPT();
 *   TDOG_PRINT("Go for coffee - this is going to take a long time.");
 *
 *   TDOG_ASSERT( is_prime(HUGE_PRIME_NUM) );
 * }
 * \endcode
 *
 * Where condition is false, it is recorded in the test report
 * as a test "fail condition". Pass conditions are recorded
 * only for "verbose test reports".
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] condition The test condition
 * \sa TDOG_ASSERT_NOT(), TDOG_ASSERT_MSG(), TDOG_ASSERT_EQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT(condition)
#else
  // Actual
  #define TDOG_ASSERT(condition) \
  tdog_helper->assert(condition, true, std::string("ASSERT( ") + \
    #condition + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that the test condition is false (test fails if
 * it is true).
 * \details This is a negative variant of TDOG_ASSERT().
 * \param[in] condition The test condition
 * \sa TDOG_ASSERT(), TDOG_ASSERT_NOT_MSG(), TDOG_ASSERT_NEQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_NOT(condition)
#else
  // Actual
  #define TDOG_ASSERT_NOT(condition) \
  tdog_helper->assert(condition, false, std::string("ASSERT_NOT( ") + \
    #condition + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that the test condition is true, and supplies a custom message
 * for the test report.
 * \details This macro is equivalent to TDOG_ASSERT() except that a custom
 * message can be supplied for use in the test report output. The message
 * will be displayed in "verbose" style reports or if the test fails.
 * \param[in] condition The test condition
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_NOT_MSG(), TDOG_ASSERT(), TDOG_ASSERT_NQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_MSG(condition, msg)
#else
  // Actual
  #define TDOG_ASSERT_MSG(condition, msg) \
  tdog_helper->assert(condition, true, msg, __LINE__)
#endif

/**
 * \brief Asserts that the test condition is false, and supplies a custom message
 * for the test report.
 * \details This is a negative variant of TDOG_ASSERT_MSG().
 * \param[in] condition The test condition
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_MSG(), TDOG_ASSERT_NOT(), TDOG_ASSERT_NEQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_NOT_MSG(condition, msg)
#else
  // Actual
  #define TDOG_ASSERT_NOT_MSG(condition, msg) \
  tdog_helper->assert(condition, false, msg, __LINE__)
#endif

/**
 * \brief Asserts that the values are equal (test fails if not).
 * \details Input values must support the equality operator.
 *
 * Where the condition is not met, a failure will be recorded in the test report.
 * Furthermore, input values will be recorded for integer, double and string
 * types. If you wish to ensure the numerical value of enum types are displayed
 * in reports, cast them to integer.
 *
 * Pass conditions are recorded only for "verbose" style reports.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] exp Expected value
 * \param[in] act Actual value
 * \sa TDOG_ASSERT_NEQ(), TDOG_ASSERT_EQ_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_EQ(exp, act)
#else
  // Actual
  #define TDOG_ASSERT_EQ(exp, act) \
  tdog_helper->assert_equal(exp, act, true, std::string("ASSERT_EQ( ") + #exp + \
    std::string(", ") + #act + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that the values are NOT equal (test fails if they are).
 * \details This is a negative variant of TDOG_ASSERT_EQ().
 * \param[in] nxp Not expected value
 * \param[in] act Actual value
 * \sa TDOG_ASSERT_EQ(), TDOG_ASSERT_NEQ_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_NEQ(nxp, act)
#else
  // Actual
  #define TDOG_ASSERT_NEQ(nxp, act) \
  tdog_helper->assert_equal(nxp, act, false, std::string("ASSERT_NEQ( ") + #nxp + \
    std::string(", ") + #act + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that 'act' is less than 'lim' (test fails if not).
 * \details Input values must support both the equality and "less than" operators.
 *
 * Where the condition is not met, a failure will be recorded in the test report.
 * Furthermore, input values will be recorded for integer, double and string
 * types. Pass conditions are recorded only for "verbose" style reports.
 *
 * Example:
 *
 * \code
 * TDOG_ASSERT_LT(func(), 10);
 * \endcode
 *
 * IMPORTANT. When comparing strings, supply at least one std::string (std::wstring)
 * type, rather than two C-style or literal strings. Otherwise the comparison will be
 * performed against the address values, rather than their content. See also
 * TDOG_ASSERT_STRIC_EQ().
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] act Actual value
 * \param[in] lim Range limit
 * \sa TDOG_ASSERT_LTE(), TDOG_ASSERT_GT(), TDOG_ASSERT_LT_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_LT(act, lim)
#else
  // Actual
  #define TDOG_ASSERT_LT(act, lim) \
  tdog_helper->assert_less_than(act, lim, false, std::string("ASSERT_LT( ") + #act + \
    std::string(", ") + #lim + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that 'act' is less than or equal to 'lim' (test fails if not).
 * \details Input values must support both the equality and "less than" operators.
 *
 * Where the condition is not met, a failure will be recorded in the test report.
 * Furthermore, input values will be recorded for integer, double and string
 * types. Pass conditions are recorded only for "verbose" style reports.
 *
 * Example:
 *
 * \code
 * TDOG_ASSERT_LTE(func(), 10);
 * \endcode
 *
 * IMPORTANT. When comparing strings, supply at least one std::string (std::wstring)
 * type, rather than two C-style or literal strings. Otherwise the comparison will be
 * performed against the address values, rather than their content. See also
 * TDOG_ASSERT_STRIC_EQ().
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] act Actual value
 * \param[in] lim Range limit
 * \sa TDOG_ASSERT_LT(), TDOG_ASSERT_GT(), TDOG_ASSERT_LTE_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_LTE(act, lim)
#else
  // Actual
  #define TDOG_ASSERT_LTE(act, lim) \
  tdog_helper->assert_less_than(act, lim, true, std::string("ASSERT_LTE( ") + #act + \
    std::string(", ") + #lim + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that 'act' is greater than 'exp' (test fails if not).
 * \details Input values must support both the equality and "greater than" operators.
 *
 * Where the condition is not met, a failure will be recorded in the test report.
 * Furthermore, input values will be recorded for integer, double and string
 * types. Pass conditions are recorded only for "verbose" style reports.
 *
 * Example:
 *
 * \code
 * TDOG_ASSERT_GT(func(), 10);
 * \endcode
 *
 * IMPORTANT. When comparing strings, supply at least one std::string (std::wstring)
 * type, rather than two C-style or literal strings. Otherwise the comparison will be
 * performed against the address values, rather than their content. See also
 * TDOG_ASSERT_STRIC_EQ().
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] act Actual value
 * \param[in] lim Range limit
 * \sa TDOG_ASSERT_GTE(), TDOG_ASSERT_LT(), TDOG_ASSERT_GT_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_GT(act, lim)
#else
  // Actual
  #define TDOG_ASSERT_GT(act, lim) \
  tdog_helper->assert_greater_than(act, lim, false, std::string("ASSERT_GT( ") + #act + \
    std::string(", ") + #lim + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that 'act' is greater than or equal to 'exp' (test fails if not).
 * \details Input values must support both the equality and "greater than" operators.
 *
 * Where the condition is not met, a failure will be recorded in the test report.
 * Furthermore, input values will be recorded for integer, double and string
 * types. Pass conditions are recorded only for "verbose" style reports.
 *
 * Example:
 *
 * \code
 * TDOG_ASSERT_GTE(func(), 10);
 * \endcode
 *
 * IMPORTANT. When comparing strings, supply at least one std::string (std::wstring)
 * type, rather than two C-style or literal strings. Otherwise the comparison will be
 * performed against the address values, rather than their content. See also
 * TDOG_ASSERT_STRIC_EQ().
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] act Actual value
 * \param[in] lim Range limit
 * \sa TDOG_ASSERT_LT(), TDOG_ASSERT_GT(), TDOG_ASSERT_LTE_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_GTE(act, lim)
#else
  // Actual
  #define TDOG_ASSERT_GTE(act, lim) \
  tdog_helper->assert_greater_than(act, lim, true, std::string("ASSERT_GTE( ") + #act + \
    std::string(", ") + #lim + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that the values are equal, and supplies a custom message
 * for the test report.
 * \details This macro is equivalent to TDOG_ASSERT_EQ() except that a
 * custom message is supplied for use in the test report output. The message
 * will be displayed in "verbose" style reports or if the test fails.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] exp Expected value
 * \param[in] act Actual value
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_NEQ_MSG(), TDOG_ASSERT_EQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_EQ_MSG(exp, act, msg)
#else
  // Actual
  #define TDOG_ASSERT_EQ_MSG(exp, act, msg) \
  tdog_helper->assert_equal(exp, act, true, msg, __LINE__)
#endif

/**
 * \brief Asserts that the values are NOT equal, and supplies a custom message
 * for the test report.
 * \details This is a negative variant of TDOG_ASSERT_EQ_MSG().
 * \param[in] nxp Not expected value
 * \param[in] act Actual value
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_EQ_MSG(), TDOG_ASSERT_NEQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_NEQ_MSG(nxp, act, msg)
#else
  // Actual
  #define TDOG_ASSERT_NEQ_MSG(nxp, act, msg) \
  tdog_helper->assert_equal(nxp, act, false, msg, __LINE__)
#endif

/**
 * \brief Asserts that 'act' is less than 'lim', and supplies a
 * custom message for the test report.
 * \details This macro is equivalent to TDOG_ASSERT_LT() except that a
 * custom message is supplied for use in the test report output. The message
 * will be displayed in "verbose" style reports or if the test fails.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] act Actual value
 * \param[in] lim Range limit
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_LTE_MSG(), TDOG_ASSERT_LT()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_LT_MSG(act, lim, msg)
#else
  // Actual
  #define TDOG_ASSERT_LT_MSG(act, lim, msg) \
  tdog_helper->assert_less_than(act, lim, false, msg, __LINE__)
#endif

/**
 * \brief Asserts that 'act' is less than or equal to 'lim', and supplies a
 * custom message for the test report.
 * \details This macro is equivalent to TDOG_ASSERT_LTE() except that a
 * custom message is supplied for use in the test report output. The message
 * will be displayed in "verbose" style reports or if the test fails.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] act Actual value
 * \param[in] lim Range limit
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_LT_MSG(), TDOG_ASSERT_LTE()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_LTE_MSG(act, lim, msg)
#else
  // Actual
  #define TDOG_ASSERT_LTE_MSG(act, lim, msg) \
  tdog_helper->assert_less_than(act, lim, true, msg, __LINE__)
#endif

/**
 * \brief Asserts that 'act' is greater than 'lim', and supplies a
 * custom message for the test report.
 * \details This macro is equivalent to TDOG_ASSERT_GT() except that a
 * custom message is supplied for use in the test report output. The message
 * will be displayed in "verbose" style reports or if the test fails.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] act Actual value
 * \param[in] lim Range limit
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_GTE_MSG(), TDOG_ASSERT_GT()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_GT_MSG(act, lim, msg)
#else
  // Actual
  #define TDOG_ASSERT_GT_MSG(act, lim, msg) \
  tdog_helper->assert_greater_than(act, lim, false, msg, __LINE__)
#endif

/**
 * \brief Asserts that 'act' is greater than or equal to 'lim', and supplies a
 * custom message for the test report.
 * \details This macro is equivalent to TDOG_ASSERT_GTE() except that a
 * custom message is supplied for use in the test report output. The message
 * will be displayed in "verbose" style reports or if the test fails.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] act Actual value
 * \param[in] lim Range limit
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_GT_MSG(), TDOG_ASSERT_GTE()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_GTE_MSG(act, lim, msg)
#else
  // Actual
  #define TDOG_ASSERT_GTE_MSG(act, lim, msg) \
  tdog_helper->assert_greater_than(act, lim, true, msg, __LINE__)
#endif

/**
 * \brief Asserts that floating point (double) values are equal
 * to within the range +/- delta.
 * \details This macro asserts that: <em>(a - delta) <= b <= (a + delta)</em>.
 *
 * It may be used to compare floating point values in cases where equality
 * is subject to precision considerations.
 *
 * Example
 *
 * \code
 * TDOG_TEST_CASE(test_sqrt)
 * {
 *   TDOG_ASSERT_DOUBLE_EQ(1.77245, my_sqrt_func(M_PI), 0.00001);
 * }
 * \endcode
 *
 * Where the condition is not met, a failure will be recorded in the test report.
 * Pass conditions are recorded only for "verbose" style reports.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] exp Expected value
 * \param[in] act Actual value
 * \param[in] delta Tolerance
 * \sa TDOG_ASSERT_DOUBLE_NEQ(), TDOG_ASSERT_DOUBLE_EQ_MSG(), TDOG_ASSERT_EQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_DOUBLE_EQ(exp, act, delta)
#else
  // Actual
  #define TDOG_ASSERT_DOUBLE_EQ(exp, act, delta) \
  tdog_helper->assert_double_equal(exp, act, true, delta, \
    std::string("ASSERT_DOUBLE_EQ( ") + #exp + std::string(", ") + \
    #act + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that floating point (double) values are NOT equal.
 * The test fails if the values are within the range +/- delta.
 * \details This is a negative variant of TDOG_ASSERT_DOUBLE_EQ().
 * \param[in] nxp Not expected value
 * \param[in] act Actual value
 * \param[in] delta Tolerance
 * \sa TDOG_ASSERT_DOUBLE_EQ(), TDOG_ASSERT_DOUBLE_NEQ_MSG(), TDOG_ASSERT_NEQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_DOUBLE_NEQ(nxp, act, delta)
#else
  // Actual
  #define TDOG_ASSERT_DOUBLE_NEQ(nxp, act, delta) \
  tdog_helper->assert_double_equal(nxp, act, false, delta, \
    std::string("ASSERT_DOUBLE_NEQ( ") + #nxp + std::string(", ") + \
    #act + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that floating point (double) values are equal
 * to within the range +/- delta. A custom message is supplied for the test report.
 * \details This macro is equivalent to TDOG_ASSERT_DOUBLE_EQ() except that a
 * custom message is supplied for use in the test report output. The message
 * will be displayed in "verbose" style reports or if the test fails.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] exp Expected value
 * \param[in] act Actual value
 * \param[in] delta Tolerance
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_DOUBLE_NEQ_MSG(), TDOG_ASSERT_DOUBLE_EQ(), TDOG_ASSERT_EQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_DOUBLE_EQ_MSG(exp, act, delta, msg)
#else
  // Actual
  #define TDOG_ASSERT_DOUBLE_EQ_MSG(exp, act, delta, msg) \
  tdog_helper->assert_double_equal(exp, act, true, delta, msg, __LINE__)
#endif

/**
 * \brief Asserts that floating point (double) values are NOT equal,
 * and supplies a custom message for the test report. The test fails if the
 * values are within the range +/- delta.
 * \details This is a negative variant of TDOG_ASSERT_DOUBLE_EQ_MSG().
 * \param[in] nxp Not expected value
 * \param[in] act Actual value
 * \param[in] delta Tolerance
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_DOUBLE_EQ_MSG(), TDOG_ASSERT_DOUBLE_NEQ(), TDOG_ASSERT_NEQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_DOUBLE_NEQ_MSG(nxp, act, delta, msg)
#else
  // Actual
  #define TDOG_ASSERT_DOUBLE_NEQ_MSG(nxp, act, delta, msg) \
  tdog_helper->assert_double_equal(nxp, act, false, delta, msg, __LINE__)
#endif

/**
 * \brief Compares strings case insensitively and asserts that
 * they are equal (test fails if not).
 * \details Where values are not equal, a failure will be
 * recorded in the test report. Pass conditions are recorded
 * only for "verbose" style reports.
 *
 * Only ASCII characters are compared case insensitively. For a
 * case sensitive comparison, use the TDOG_ASSERT_EQ() macro.
 *
 * Input parameters are cast to the std::string or
 * std::wstring type, as applicable. Because these classes
 * accept C-style strings in their constructors, literal strings
 * can also be used as input values.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] exp Expected value
 * \param[in] act Actual value
 * \sa TDOG_ASSERT_STRIC_NEQ(), TDOG_ASSERT_STRIC_EQ_MSG(), TDOG_ASSERT_EQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_STRIC_EQ(exp, act)
#else
  // Actual
  #define TDOG_ASSERT_STRIC_EQ(exp, act) \
  tdog_helper->assert_stric_equal(exp, act, true, \
    std::string("ASSERT_STRIC_EQ( ") + #exp + \
    std::string(", ") + #act + std::string(" )"), __LINE__)
#endif

/**
 * \brief Compares strings case insensitively and asserts that
 * they are NOT equal (test fails if they are).
 * \details This is a negative variant of TDOG_ASSERT_STRIC_EQ().
 *
 * Only ASCII characters are compared case insensitively. For a
 * case sensitive comparison, use the TDOG_ASSERT_EQ() macro.
 *
 * Input parameters are cast to the std::string or
 * std::wstring type, as applicable. Because these classes
 * accept C-style strings in their constructors, literal strings
 * can also be used as input values.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] nxp Not expected value
 * \param[in] act Actual value
 * \sa TDOG_ASSERT_STRIC_EQ(), TDOG_ASSERT_STRIC_NEQ_MSG(), TDOG_ASSERT_NEQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_STRIC_NEQ(nxp, act)
#else
  // Actual
  #define TDOG_ASSERT_STRIC_NEQ(nxp, act) \
  tdog_helper->assert_stric_equal(nxp, act, false, \
    std::string("ASSERT_STRIC_NEQ( ") + #nxp + \
    std::string(", ") + #act + std::string(" )"), __LINE__)
#endif

/**
 * \brief Compares strings case insensitively and asserts that
 * they are equal. A custom message is supplied for the test report.
 * \details This macro is equivalent to TDOG_ASSERT_STRIC_EQ() except that a
 * custom message is supplied for use in the test report output. The message
 * will be displayed in "verbose" style reports or if the test fails.
 *
 * Only ASCII characters are compared case insensitively. For a
 * case sensitive comparison, use the TDOG_ASSERT_EQ() macro.
 *
 * Input parameters are cast to the std::string or
 * std::wstring type, as applicable. Because these classes
 * accept C-style strings in their constructors, literal strings
 * can also be used as input values.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] exp Expected value
 * \param[in] act Actual value
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_STRIC_NEQ_MSG(), TDOG_ASSERT_STRIC_EQ(), TDOG_ASSERT_EQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_STRIC_EQ_MSG(exp, act, msg)
#else
  // Actual
  #define TDOG_ASSERT_STRIC_EQ_MSG(exp, act, msg) \
  tdog_helper->assert_stric_equal(exp, act, true, msg, __LINE__)
#endif

/**
 * \brief Compares strings case insensitively and asserts that
 * they are NOT equal. A custom message is supplied for the test report.
 * \details This is a negative variant of TDOG_ASSERT_STRIC_EQ_MSG().
 *
 * Only ASCII characters are compared case insensitively. For a
 * case sensitive comparison, use the TDOG_ASSERT_EQ() macro.
 *
 * Input parameters are cast to the std::string or
 * std::wstring type, as applicable. Because these classes
 * accept C-style strings in their constructors, literal strings
 * can also be used as input values.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] nxp Not expected value
 * \param[in] act Actual value
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_STRIC_EQ_MSG(), TDOG_ASSERT_STRIC_NEQ(), TDOG_ASSERT_NEQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_STRIC_NEQ_MSG(nxp, act, msg)
#else
  // Actual
  #define TDOG_ASSERT_STRIC_NEQ_MSG(nxp, act, msg) \
  tdog_helper->assert_stric_equal(nxp, act, false, msg, __LINE__)
#endif

/**
 * \brief Asserts that the content of array contents are equal (the test
 * fails if not).
 * \details This macro will compare up to "size" array elements individually,
 * and the array item type must support the equality operator to facilitate the
 * comparision. Where the condition is not met, a failure will be recorded in the
 * test report. Pass conditions are recorded only for "verbose" style reports.
 *
 * Example:
 *
 * \code
 * int my_sqr(int x) { return x * x; }
 *
 * TDOG_TEST_CASE(test_array)
 * {
 *   int a[] = {1, 4, 9};
 *   int b[] = {my_sqr(1), my_sqr(2), my_sqr(3)};
 *   TDOG_ASSERT_ARRAY_EQ(a, b, 3);
 * }
 * \endcode
 *
 * Both arrays must contain at least "size" items.
 *
 * Any data type which which can be indexed using the [] operator,
 * such as std::vector, can be compared using this macro. A null
 * pointer passed as a C-style type with a non-zero "size" value will
 * cause the test to fail.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] exp Expected value
 * \param[in] act Actual value
 * \param[in] size Number of elements to be compared
 * \sa TDOG_ASSERT_ARRAY_NEQ(), TDOG_ASSERT_ARRAY_EQ_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_ARRAY_EQ(exp, act, size)
#else
  // Actual
  #define TDOG_ASSERT_ARRAY_EQ(exp, act, size) \
  tdog_helper->assert_array_equal(exp, act, true, size, \
  std::string("ASSERT_ARRAY_EQ( ") + #exp + \
    std::string(", ") + #act + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that the content of array contents are NOT equal (the test
 *  fails if they are).
 * \details This is a negative variant of TDOG_ASSERT_ARRAY_EQ().
 *
 * Both arrays must contain at least "size" items.
 *
 * Any data type which which can be indexed using the [] operator,
 * such as std::vector, can be compared using this macro. A null
 * pointer passed as a C-style type with a non-zero "size" value will
 * cause the test to fail.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] nxp Not expected value
 * \param[in] act Actual value
 * \param[in] size Number of elements to be compared
 * \sa TDOG_ASSERT_ARRAY_EQ(), TDOG_ASSERT_ARRAY_NEQ_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_ARRAY_NEQ(nxp, act, size)
#else
  // Actual
  #define TDOG_ASSERT_ARRAY_NEQ(nxp, act, size) \
  tdog_helper->assert_array_equal(nxp, act, false, size, \
  std::string("ASSERT_ARRAY_NEQ( ") + #nxp + \
    std::string(", ") + #act + std::string(" )"), __LINE__)
#endif

/**
 * \brief Asserts that the content of arrays are equal (the test
 *  fails if not). A custom message is supplied for the test report.
 * \details This macro is equivalent to TDOG_ASSERT_ARRAY_EQ() except that a
 * custom message is supplied for use in the test report output. The message
 * will be displayed in "verbose" style reports or if the test fails.
 *
 * Any data type which which can be indexed using the [] operator,
 * such as std::vector, can be compared using this macro. A null
 * pointer passed as a C-style type with a non-zero "size" value will
 * cause the test to fail.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] exp Expected value
 * \param[in] act Actual value
 * \param[in] size Number of items to be compared
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_ARRAY_NEQ_MSG(), TDOG_ASSERT_ARRAY_EQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_ARRAY_EQ_MSG(exp, act, size, msg)
#else
  // Actual
  #define TDOG_ASSERT_ARRAY_EQ_MSG(exp, act, size, msg) \
  tdog_helper->assert_array_equal(exp, act, true, size, msg, __LINE__)
#endif

/**
 * \brief Asserts that the content of arrays are NOT equal (the test
 *  fails if they are). A custom message is supplied for the test report.
 * \details This is a negative variant of TDOG_ASSERT_ARRAY_EQ_MSG().
 *
 * Any data type which which can be indexed using the [] operator,
 * such as std::vector, can be compared using this macro. A null
 * pointer passed as a C-style type with a non-zero "size" value will
 * cause the test to fail.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] nxp Not expected value
 * \param[in] act Actual value
 * \param[in] size Number of items to be compared
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_ASSERT_ARRAY_EQ_MSG(), TDOG_ASSERT_ARRAY_NEQ()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_ARRAY_NEQ_MSG(nxp, act, size, msg)
#else
  // Actual
  #define TDOG_ASSERT_ARRAY_NEQ_MSG(nxp, act, size, msg) \
  tdog_helper->assert_array_equal(nxp, act, false, size, msg, __LINE__)
#endif

/**
 * \brief Asserts that the expression throws an exception of the
 * specified type.
 * \details The test fails if the exception is NOT thrown.
 *
 * Example:
 *
 * \code
 * // Test will fail if exception not thrown
 * TDOG_ASSERT_THROW( my_sqrt(-36) , std::exception );
 * \endcode
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] exprn Test expression
 * \param[in] e_type Test exception type
 * \sa TDOG_ASSERT_NO_THROW()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_THROW(exprn, e_type)
#else
  // Actual
  // The while(false) allows statement to be terminated with semicolon
  #define TDOG_ASSERT_THROW(exprn, e_type) \
  do { \
    bool ethrown = false; \
    try {  exprn; } \
    catch(const e_type&) { ethrown = true; } \
    catch(...) {} \
    tdog_helper->assert(ethrown, true, \
      std::string("ASSERT THROWS ( ") + #exprn + \
      std::string(" )") + #e_type, __LINE__); \
  } while(false)
#endif

/**
 * \brief Asserts that the expression does not throw an exception.
 * \details The test fails if any exception is thrown.
 *
 * Example:
 *
 * \code
 * // Test will fail if exception is thrown
 * TDOG_ASSERT_NO_THROW( my_sqrt(36) );
 * \endcode
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] exprn Test expression
 * \sa TDOG_ASSERT_THROW()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ASSERT_NO_THROW(exprn)
#else
  // Actual
  // The while(false) allows statement to be terminated with semicolon
  #define TDOG_ASSERT_NO_THROW(exprn) \
  do { \
    bool ethrown = false; \
    try {  exprn; } \
    catch(...) { ethrown = true; } \
    tdog_helper->assert(ethrown, false, \
      std::string("ASSERT NO THROW ( ") + #exprn + \
      std::string(" )"), __LINE__); \
  } while(false)
#endif

/**
 * \brief Puts the test into the failure state, and writes the supplied message
 * to the test report.
 * \details The message will be displayed in-line at the point it is occurs,
 * irrespective of whether the report is verbose or not.
 *
 * Test execution continues after this call, as a test case may
 * implement multiple condition checks.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_PRINT(), TDOG_TEST_WARNING(), TDOG_TEST_ERROR(), TDOG_ASSERT_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_TEST_FAIL(msg)
#else
  // Actual
  #define TDOG_TEST_FAIL(msg) \
  tdog_helper->raise_failure(msg, __LINE__)
#endif

/**
 * \brief Terminates test execution, and puts the test into an error state.
 * \details A test case will automically be placed in the "error state" if the
 * test implementation throws an unhandled exception. The TDOG_TEST_ERROR() macro,
 * however, can be used to explicitly put the test into an error state from within
 * the test code, at which point the test will terminate.
 *
 * The message will be displayed in-line at the point it is occurs,
 * irrespective of whether the report is verbose or not.
 *
 * A "test error" is considered distinct from a "test failure" because it
 * explicitly refers to an unhandled exception or implementation problem
 * in the test code.
 *
 * \code
 * TDOG_TEST_CASE(add_customer_record)
 * {
 *   // Create initial DB records needed to perform test
 *   if (!some_condition)
 *   {
 *     // The test will terminate after this call
 *     TDOG_TEST_ERROR("Condition not met - could not perform test");
 *   }
 *
 *   TDOG_PRINT("Things are ok - begin real part of test");
 *   ...
 * }
 * \endcode
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_TEST_FAIL(), TDOG_PRINT(), TDOG_TEST_WARNING(), TDOG_ASSERT_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_TEST_ERROR(msg)
#else
  // Actual
  #define TDOG_TEST_ERROR(msg) \
  tdog_helper->raise_error(msg, __LINE__); return
#endif

/**
 * @}
 * @name Test message output
 * @{
*/

/**
 * \brief Writes an information message string to the test report.
 * \details Information messages appear only in verbose style reports,
 * such as RS_TEXT_VERBOSE. They are ignored for reports which are not
 * considered to be verbose.
 *
 * Example
 *
 * \code
 * TDOG_TEST_CASE(is_prime_func)
 * {
 *   TDOG_SET_GLOBAL_LIMIT_EXEMPT();
 *   TDOG_PRINT("Go for coffee - this is going to take a long time.");
 *
 *   TDOG_ASSERT( is_prime(HUGE_PRIME_NUM) );
 * }
 * \endcode
 *
 * To write out a formatted value to the report, use TDOG_PRINTF() instead.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_PRINTF(), TDOG_TEST_WARNING(), TDOG_TEST_ERROR(), TDOG_ASSERT_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_PRINT(msg)
#else
  // Actual
  #define TDOG_PRINT(msg) \
  tdog_helper->print(msg, __LINE__)
#endif

/**
 * \brief Writes a formatted value information string to the test report.
 * \details This is macro is similar to TDOG_PRINT(), except that it is
 * possible to supply an additional value which will appear in a formatted
 * message. The format is similar to that used in the C printf() function,
 * although more restrictive.
 *
 * The "val" parameter may be an any integer type upto 64 bit in size, or
 * one of the following types:
 * - const char*
 * - const wchar_t*
 * - std::string
 * - std::wstring
 * - double
 * .
 * To identify the location and format of the value in the output, the
 * message string should contain one of the format specifiers below.
 *
 * Integer Values:
 * - \%d or \%i - Signed integer
 * - \%u - Unsigned integer
 * - \%c - Character
 * - \%o - Signed octal integer
 * - \%x - Unsigned hex integer (lower case)
 * - \%02x - 2 digit hex (lower case)
 * - \%04x - 4 digit hex (lower case)
 * - \%08x - 8 digit hex (lower case)
 * - \%X - Unsigned hex (upper case)
 * - \%02X - 2 digit hex (upper case)
 * - \%04X - 4 digit hex (upper case)
 * - \%08X - 8 digit hex (upper case)
 * - \%b - Boolean ("true", or "false")
 * .
 * Floating point:
 * - \%f - Double
 * .
 * String:
 * - \%s - String value
 * - \%k - Escaped string value
 * .
 * Generic:
 * - \%g - Defaults to \%d, \%f, or \%s depending on value type
 *
 * It is recommended that you use "\%g" generally, unless a specific format is
 * required, as it will act as the placeholder for all supported value types.
 *
 * Example
 *
 * \code
 * TDOG_PRINTF("final result is 0x%08X", rslt);
 * \endcode
 *
 * Unlike the C printf() function, the TDOG_PRINTF() macro does not
 * support a variable number of arguments, and the available
 * specifiers are restricted to the exact forms above.
 *
 * Additionally, the "%b" and "%k" specifiers have no equivalent in the
 * C printf() function. The "%b" is used to represent a boolean value,
 * and is substituted with either "true" or "false". The "\%k" applies
 * to strings, but unlike "\%s", ASCII characters less than 0x20
 * (space) in value will be substitued with "escaped" characters
 * including the replacements: "\t", "\n", "\v", "\f" and "\r".
 *
 * For example, the tab character (0x09) will be replaced with "\t".
 * For other characters, hex digits will be used instead, such as "\x08"
 * for backspace.
 *
 * Where the input message contains multiple specifiers,
 * all will be substituted with the same value.
 *
 * Example
 *
 * \code
 * TDOG_PRINTF("final result: %d, HEX: 0x%08X, OCT:0%o", 34283);
 * \endcode
 *
 * Gives the following message output:
 * \code
 * final result: 34283, HEX: 0x000085EB, OCT:0102753
 * \endcode
 *
 * To output the numerical value of enum types, cast them to integer.
 *
 * As with TDOG_PRINT(), value formatted messages appear only in verbose
 * style reports (i.e. RS_TEXT_VERBOSE). They are ignored for reports which
 * are not considered to be verbose.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] msg Message passed as std::string or C-style string
 * \param[in] val Value
 * \sa TDOG_PRINT(), TDOG_TEST_WARNING(), TDOG_TEST_ERROR(), TDOG_ASSERT_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_PRINTF(msg, val)
#else
  // Actual
  #define TDOG_PRINTF(msg, val) \
  tdog_helper->printf(msg, val, __LINE__)
#endif

/**
 * \brief Writes a custom warning message to the test report.
 * \details This macro writes a warning message to the test reports.
 * The message will be displayed in-line at the point it is occurs,
 * irrespective of whether the report is verbose or not.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] msg Message passed as std::string or C-style string
 * \sa TDOG_TEST_FAIL(), TDOG_PRINT(), TDOG_TEST_ERROR(), TDOG_ASSERT_MSG()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_TEST_WARNING(msg)
#else
  // Actual
  #define TDOG_TEST_WARNING(msg) \
  tdog_helper->raise_warning(msg, __LINE__)
#endif

/**
 * @}
 * @name Running tests
 * @{
 */

/**
 * \brief Run the tests matching the given list of test case and suite name(s).
 * \details The input string may contain multiple names separated with a space or
 * comma (or both). Test case names should be qualified with the suite name, and
 * are case sensitive. If a test case is not part of a suite (i.e. it's in the
 * default suite), then the name may optionally be prefixed with the default
 * namespace "::". Any name referring to a test or suite which does not exist
 * will be ignored.
 *
 * Both test case and suite names may be specified, and will match according to the
 * following rules:
 *
 * - To match a single test, provide the fully qualified test name exactly
 * (i.e. "SUITE1::SUITE2::TESTNAME").
 * - To match all tests directly in suite, but not those in any of its sub-suites,
 * use the suite name suffixed with "::". For example, "SUITE1::".
 * - To match all tests in a suite, including its sub-suites, suffix the name
 * input with "::*". For example, "SUITE1::*".
 * - To match everything, use "*" (or "::*").
 * .
 *
 * To execute all tests (in all suites), specify "*" as the input, as follows:
 *
 * \code
 * // Run all tests
 * TDOG_RUN("*");
 * \endcode
 *
 * To run a single test case in the default suite, for example, call:
 *
 * \code
 * // Run a single test
 * TDOG_RUN("TESTNAME1");
 * \endcode
 *
 * To specify all tests in the default test suite (i.e. those declared outside of
 * a suite) use "::". For example, the following will run the default suite,
 * plus a single test in another suite:
 *
 * \code
 * // Run specified tests
 * TDOG_RUN("::, SUITE1::TESTNAME2");
 * \endcode
 *
 * Test cases which have been disabled will be not be executed, even if they
 * are explicity specified in the "names" string.
 *
 * When running tests, suite names will always be sorted, with the default suite
 * executed first. Names are sorted according to an ordinal* comparison between their
 * ASCII lower case values, and if equal, their case sensitive values. This will execute
 * names with case variations together in a consistent way.
 *
 * By default, test names within suites will be executed in the order they are
 * registered with the runner, with a few exceptions:
 *
 * - Test names beginning with "setup" or "_setup" (case insensitive) will always
 * be executed at the beginning of the suite.
 * - Test names beginning with "teardown" or "_teardown" (case insensitive) will
 * always be executed at the end of the suite -- after any nested suites.
 * .
 *
 * If TDOG_SET_RUN_SORTED() is called to fully sort the run order, test cases inside suites
 * will also be sorted during execution. It is good practice to ensure that tests do not
 * depend on the run order, other than for explicitly declared setup() and teardown()
 * operations.
 *
 * The return value of TDOG_RUN() gives the number of test cases which either failed or
 * raised an error. The result will be 0 if all test cases ran successfully without
 * asserting failures or test errors. If, however, no tests were run, the
 * return value will be tdog::RAN_NONE (-1), rather than 0. This distinguishes
 * a successful test run from one that didn't actually test anything.
 * This may occur, for example, if all test cases are disabled, if no
 * tests have been registered with the runner, or the name input refers
 * to non-existent tests.
 *
 * This macro should not be called from within a test.
 * \param[in] names Separated list of names passed as std::string or C-style string
 * \return Number of test failures, or tdog::RAN_NONE (-1) if no tests performed
 * \sa TDOG_RUN_ALL(), TDOG_SET_RUN_SORTED()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_RUN(names)
#else
  // Actual
  #define TDOG_RUN(names) \
  tdog::runner::global().run(names, false)
#endif

/** \brief Run all tests -- a synonym for TDOG_RUN("*").
 * \return Number of test failures, or tdog::RAN_NONE (-1) if no tests performed
 * \sa TDOG_RUN()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_RUN_ALL()
#else
  // Actual
  #define TDOG_RUN_ALL() \
  tdog::runner::global().run("*", false)
#endif

/**
 * \brief Run the tests according to the given command line arguments.
 * \details This macro takes the command line arguments provided by a main()
 * function, and runs the tests according to specification:
 *
 * \code
 * --trun <run test list> [--tdis <disable test list>]
 * \endcode
 *
 * Other parameters will be ignored.
 *
 * To run all tests, the command "--trun *" should be given or,
 * alternatively "--trall". The use of "--dis" to disable tests is optional.
 *
 * For example, the following will run all with two tests disabled:
 *
 * \code
 * --trun * --tdis test1,suite2::test2
 * \endcode
 *
 * Windows style input, with a command prefix of "/", is also accepted
 * and the following command is equivalent to that above:
 *
 * \code
 * /trun:* /tdis:test1,suite2::test2
 * \endcode
 *
 * This macro is also overloaded with the wchar_t type for use with a
 * Windows main function.
 *
 * If "def_run" is true, all tests will execute by default if no "--trun"
 * or "--trall" command is found. In this case, tests may optionally be
 * disabled with the "--tdis" command alone.
 *
 * The return value gives the number of test cases which either failed or raised
 * an error. The result will be 0 if all test cases ran successfully without
 * asserting failures or test errors. If, however, no tests were run,
 * the return value will be tdog::RAN_NONE (-1), rather than 0. This distinguishes
 * a successful test run from one that doesn't actually test anything. In the event
 * that the argument list does not contain "-trun" or "-trall", the result will also
 * be tdog::RAN_NONE (-1) unless "def_run" is true.
 *
 * This macro should not be called from within a test.
 * \param[in] argc Argument count
 * \param[in] argv Pointer to array of c-strings from main()
 * \param[in] def_run Boolean flag, run tests by default.
 * \return Number of test failures, or RUN_NONE (-1) if no tests performed
 * \sa TDOG_RUN()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_RUN_CMD(argc, argv, def_run)
#else
  // Actual
  #define TDOG_RUN_CMD(argc, argv, def_run) \
  tdog::runner::global().run_cmdline(argc, argv, def_run)
#endif

/**
 * @}
 * @name Test run properties
 * @{
 */

/**
 * \brief Sets whether the run order is fully sorted.
 * \details When running tests, suite names will always be sorted, with the default
 * suite first. Names are sorted according to an ordinal* comparison between their ASCII
 * lower case values, and if equal, their case sensitive values. This will sort
 * names with case variations together in a consistent way.
 *
 * By default, test names within suites will be executed in the order they are
 * registered with the runner, with a few exceptions:
 *
 * - Test names beginning with "setup" or "_setup" (case insensitive) will always
 * be executed at the beginning of the suite.
 * - Test names beginning with "teardown" or "_teardown" (case insensitive) will
 * always be executed at the end of the suite -- after any nested suites.
 * .
 *
 * If the "full_sort" option is true prior to execution, test cases inside suites will
 * also be sorted during execution. It is good practice to ensure that tests do not
 * depend on the run order, other than for explicitly declared setup() and teardown()
 * operations.
 *
 * Example:
 *
 * \code
 * // Set sorted run order
 * TDOG_SET_RUN_SORTED(true);
 *
 * // Run the tests
 * TDOG_RUN("*");
 * \endcode
 *
 * *In an ordinal sort, "setup2" will come after "setup10". Use additional zeros
 * to ensure numerical order, i.e. "setup02" and "setup10", if required.
 *
 * This macro should not be called from within a test.
 * \param[in] full_sort Set true for a fully sorted run order (default is false)
 * \sa TDOG_RUN()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SET_RUN_SORTED(full_sort)
#else
  // Actual
  #define TDOG_SET_RUN_SORTED(full_sort) \
  tdog::runner::global().set_sorted(full_sort)
#endif

/**
 * \brief Sets the name string for the software under test.
 * \details Use to give the test project a name (typically your software's name) prior to
 * test execution. This will be displayed in test reports. Its use is optional.
 *
 * Example:
 *
 * \code
 * // Set project name and version
 * TDOG_SET_PROJECT_NAME("smppd");
 * TDOG_SET_PROJECT_VERSION("3.1.6");
 *
 * // Run the tests
 * TDOG_RUN("*");
 * \endcode
 *
 * This macro should not be called from within a test.
 * \param[in] name Project name string
 * \sa TDOG_SET_PROJECT_VERSION(), TDOG_SET_PROJECT_DESC()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SET_PROJECT_NAME(name)
#else
  // Actual
  #define TDOG_SET_PROJECT_NAME(name) \
  tdog::runner::global().set_project_name(name)
#endif

/**
 * \brief Sets version number string of the software under test.
 * \details The version will be displayed along with
 * project name in test reports. Its use is optional.
 *
 * Example:
 *
 * \code
 * // Set project name and version
 * TDOG_SET_PROJECT_NAME("smppd");
 * TDOG_SET_PROJECT_VERSION("3.1.6");
 *
 * // Run the tests
 * TDOG_RUN("*");
 * \endcode
 *
 * This macro should not be called from within a test.
 * \param[in] version Project version string
 * \sa TDOG_SET_PROJECT_NAME(), TDOG_SET_PROJECT_DESC()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SET_PROJECT_VERSION(version)
#else
  // Actual
  #define TDOG_SET_PROJECT_VERSION(version) \
  tdog::runner::global().set_project_version(version)
#endif

/**
 * \brief Sets a descriptive string of the project under test.
 * \details The project description is an optional string which is displayed
 * in test report outputs.
 * \param[in] desc Project description string
 * \sa TDOG_SET_PROJECT_NAME(), TDOG_SET_PROJECT_VERSION()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SET_PROJECT_DESC(desc)
#else
  // Actual
  #define TDOG_SET_PROJECT_DESC(desc) \
  tdog::runner::global().set_project_desc(desc)
#endif

/**
 * \brief Sets the global time constraint per test in milliseconds.
 * \details This is a global constraint on the maximum amount of time
 * that each test case may take to complete. If a test case takes
 * longer than this time, it will be deemed to have failed. This
 * macro should be called prior to running the tests. A value
 * of 0, the default, disables the global time constaint.
 *
 * This macro should not be called from within a test.
 *
 * Time constraints may also be applied to test cases on an individual
 * basis. See TDOG_SET_TEST_TIMEOUT().
 * \param[in] ms Time constaint in milliseconds
 * \sa TDOG_SET_TEST_TIMEOUT(), TDOG_SET_GLOBAL_TIME_WARNING()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SET_GLOBAL_TIMEOUT(ms)
#else
  // Actual
  #define TDOG_SET_GLOBAL_TIMEOUT(ms) \
  tdog::runner::global().set_global_time_limit(ms)
#endif

/**
 * \brief Sets the global warning time per test in milliseconds.
 * \details If a test takes longer than this time, a warning message
 * will be generated in the test report. However, the test will not fail
 * because of this.
 *
 * It is possible to set both a warning time, and a longer global
 * time contraint, for example. This would mean that tests approaching
 * the limit generate warnings, while any exceeding it will fail.
 *
 * This macro should not be called from within a test.
 * \param[in] ms Time in milliseconds
 * \sa TDOG_SET_GLOBAL_TIMEOUT()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SET_GLOBAL_TIME_WARNING(ms)
#else
  // Actual
  #define TDOG_SET_GLOBAL_TIME_WARNING(ms) \
  tdog::runner::global().set_global_time_warning(ms)
#endif

/**
 * @}
 * @name Reporting
 * @{
 */

/**
 * \brief Sets the report style written to STDOUT during the test run.
 * \details If required, this macro should called prior to running the tests.
 * The report "style" parameter can be one of the following tdog::report_style_t
 * values:
 * - tdog::RS_NONE - No report is generated
 * - tdog::RS_TEXT_SUMMARY - Human readable test summary
 * - tdog::RS_TEXT_VERBOSE - Human readable verbose test report
 * - tdog::RS_HTML_SUMMARY - HTML summary report
 * - tdog::RS_HTML_VERBOSE - HTML verbose report
 * - tdog::RS_XML - JUnit compatible XML report.
 * .
 * The default value is RS_TEXT_SUMMARY.
 *
 * Non-verbose reports contain only summary information pertaining to tests which pass,
 * but further details are provided for those tests which fail. Verbose reports, on the
 * other hand, contain maximum trace information. For verbose XML, the report
 * will test case output information within the "<system-out>" element. HTML report content
 * is suitable for viewing in a browser.
 *
 * Example:
 *
 * \code
 * // Use verbose report
 * TDOG_SET_DEFAULT_REPORT(tdog::RS_TEXT_VERBOSE);
 * ...
 * TDOG_RUN("*");
 * \endcode
 *
 * You can use the tdog::RS_NONE to silence TDOG's own output to STDOUT, but this
 * won't affect output generated by any printf() or std::cout statements within
 * test implementation code. It you specify a non-text report style, you would typically
 * want to re-direct the process output to file, but see also the TDOG_ADD_REPORT() macro.
 *
 * This macro should not be called from within a test.
 * \param[in] style Report style
 * \sa TDOG_ADD_REPORT()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SET_DEFAULT_REPORT(style)
#else
  // Actual
  #define TDOG_SET_DEFAULT_REPORT(style) \
  tdog::runner::global().set_default_report(style)
#endif

/**
 * \brief Causes an additional report to be written to file automatically
 * at the conclusion of the test run.
 * \details During the test run, a text summary report is normally written to
 * STDOUT as tests are performed. This macro can be used to specify that an additional
 * report should be written to file when the test run has finished. The "filename"
 * parameter specifies the name of the file to write, while the report style parameter
 * can be one of the following tdog::report_style_t values:
 * - tdog::RS_NONE - Does nothing here
 * - tdog::RS_TEXT_SUMMARY - Human readable test summary
 * - tdog::RS_TEXT_VERBOSE - Human readable verbose test report
 * - tdog::RS_HTML_SUMMARY - HTML summary report
 * - tdog::RS_HTML_VERBOSE - HTML verbose report
 * - tdog::RS_XML - JUnit compatible XML report.
 * .
 * Non-verbose reports contain only summary information pertaining to tests which pass,
 * but further details are provided for those tests which fail. Verbose reports, on the
 * other hand, contain maximum trace information. For verbose XML, the report
 * will test case output information within the "<system-out>" element. HTML reports
 * are suitable for viewing in a browser.
 *
 * By default, no additional reports are written to file.
 *
 * Here's an example of where the default reporting to STDOUT is silenced,
 * but verbose TEXT, HTML and XML reports are written to file after the test run:
 *
 * \code
 * // Silence STDOUT report
 * TDOG_SET_DEFAULT_REPORT(tdog::RS_NONE);
 *
 * // Add TEXT report
 * TDOG_ADD_REPORT(tdog::RS_TEXT_VERBOSE, "./test-report.txt");
 *
 * // Add HTML report
 * TDOG_ADD_REPORT(tdog::RS_HTML_VERBOSE, "./test-report.html");
 *
 * // Add XML report
 * TDOG_ADD_REPORT(tdog::RS_XML, "./test-report.xml");
 *
 * // Run all tests
 * TDOG_RUN("*");
 * \endcode
 *
 * If the style RS_NONE is used, this macro does nothing.
 *
 * This macro should not be called from within a test.
 * \param[in] style Report style
 * \param[in] filename Output report filename string
 * \sa TDOG_SET_DEFAULT_REPORT()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_ADD_REPORT(style, filename)
#else
  // Actual
  #define TDOG_ADD_REPORT(style, filename) \
  tdog::runner::global().add_report(style, filename)
#endif

/**
 * @}
 * @name Accessing statistics
 * @{
 */

/**
 * \brief Returns a statistical result for the test case, suite or entire test run.
 * \details This method should be called after the executing the tests, and
 * the result will pertain to the following statistic type:
 * - tdog::CNT_TOTAL - Total number of registered tests matching the name
 * - tdog::CNT_RAN - Number of test cases executed
 * - tdog::CNT_PASSED - Number of tests which passed (inc. warnings)
 * - tdog::CNT_FAILED - Number of test cases which failed
 * - tdog::CNT_ERRORS - Number of test cases with implementation errors
 * - tdog::CNT_SKIPPED - Number of test cases skipped (exc. disabled)
 * - tdog::CNT_DISABLED - Number of disabled test cases (exc. skipped)
 * - tdog::CNT_WARNINGS - Number of tests with warnings
 * - tdog::CNT_PASS_RATE - Percentage of tests which passed
 * - tdog::CNT_ASSERT_TOTAL - Total number of assert statements
 * - tdog::CNT_ASSERT_FAILED - Number of assert failures
 * - tdog::CNT_DURATION - Duration in milliseconds
 * .
 * Either a single test case or suite name may be specified, and will match tests
 * according to the following rules:
 *
 * - To match a single test, provide the fully qualified test name exactly
 * (i.e. "SUITE1::SUITE2::TESTNAME").
 * - To match all tests directly in suite, but not in any of its sub-suites,
 * use the suite name suffixed with "::". For example, "SUITE1::".
 * - To match all tests in a suite, including its sub-suites, suffix the name
 * input with "::*". For example, "SUITE1::*".
 * - To match everything, use "*" (or "::*").
 * .
 *
 * Note that this routine does not support multiple name input. Provide only a
 * single name according to the rules above.
 *
 * Example:
 *
 * \code
 * // Get count of all passed tests
 * int pass = TDOG_STATISTIC_COUNT(tdog::CNT_PASSED, "*");
 * \endcode
 *
 * Note also that the "ASSERT" result types pertain explicitly to counts of
 * TDOG_ASSERT() statements (and variants of). They are not incremented, for
 * example, if the test fails because of a timeout error, or if TDOG_TEST_FAIL()
 * is called.
 *
 * All return values will be zero if no test run was performed prior to calling
 * this method, otherwise they will be an applicable count value or duration.
 * \param[in] type Required statistic of type
 * \param[in] name Test case or suite name string, or "*" for all
 * \return Statistical value
 * \sa TDOG_TEST_STATUS()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_STATISTIC_COUNT(type, name)
#else
  // Actual
  #define TDOG_STATISTIC_COUNT(type, name) \
  tdog::runner::global().statistic_count(type, name)
#endif

/**
 * \brief Returns the state of a given test case.
 * \details Given a fully qualified test name, this macro returns one of the
 * following values:
 * - tdog::TS_NOT_EXIST - Test case does not exist
 * - tdog::TS_READY - Test is ready to run
 * - tdog::TS_PASS_OK - Test passed OK
 * - tdog::TS_PASS_WARN - Test passed with warning
 * - tdog::TS_FAILED - Test failed
 * - tdog::TS_ERROR - Test failed because of a possible test case implementation error
 * - tdog::TS_SKIPPED - Test is enabled, but was skipped because by the author
 * - tdog::TS_DISABLED - Test is disabled
 * - tdog::TS_DECL_ERROR - Test run aborted because of test declaration errors
 * .
 * If the test case was declared within a test suite, if should be qualified
 * with the suite name using the "::" as the separator. If a test case is not
 * part of a suite (i.e. it's in the default suite), then the name may optionally
 * be prefixed with the default namespace "::".
 *
 * This macro pertains to individual test cases only, and will not return data
 * for suites. Test names are case sensitive, and multiple names are not
 * supported.
 * \param[in] name Test case name string
 * \return Test result state of type std::test_status_t
 * \sa TDOG_STATISTIC_COUNT()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_TEST_STATUS(name)
#else
  // Actual
  #define TDOG_TEST_STATUS(name) \
  tdog::runner::global().test_status(name)
#endif

/**
 * @}
 * @name Disabling tests
 * @{
 */

/** \brief Allows test cases and suites of tests matching the given name list
 * to be disabled (or re-enabled).
 * \details Disabled tests will not be executed by the test run.
 *
 * The input string may contain multiple names separated with a space or
 * comma (or both). Test case names should be fully qualified with its suite name.
 * If a test case is not part of a suite (i.e. it's in the default suite), then
 * the name may optionally be prefixed with the default namespace "::".
 *
 * Both test case and suite names may be specified, and will match according to the
 * following rules:
 *
 * - To match a single test, provide the fully qualified test name exactly
 * (i.e. "SUITE1::SUITE2::TESTNAME").
 * - To match all tests directly in suite, but not in any of its sub-suites,
 * use the suite name suffixed with "::". For example, "SUITE1::".
 * - To match all tests in a suite, including its sub-suites, suffix the name
 * input with "::*". For example, "SUITE1::*".
 * - To match everything, use "*" (or "::*").
 * .
 *
 * Example:
 *
 * \code
 * TDOG_SET_ENABLED("TEST1, SUITE1::, SUITE2::TEST2", false);
 * \endcode
 *
 * Here, "TEST1" is a test case declared outside of a suite, and will be disabled.
 * Other tests in the default suite will not be affected. All tests directly under
 * "SUITE1" will be disabled, but not those under sub-suites of "SUITE1". Finally,
 * "TEST2" will be disabled in "SUITE2".
 *
 * To refer to the default suite, use "::" as the input. For example, the
 * following will disable all tests declared outside of any suite:
 *
 * \code
 * TDOG_SET_ENABLED("::", false);
 * \endcode
 *
 * Names are case sensitive, and any name which does not exist will be ignored.
 * The result is the total number of test cases updated. Use TDOG_TEST_STATUS() to
 * discover the enabled state of a test.
 *
 * The diffence between specifically disabling a test and omitting it when calling
 * the TDOG_RUN() macro is that disabled tests will be noted in the test report,
 * whereas omitted tests will simply be ignored by the run call.
 *
 * This macro should not be called from within a test.
 * \param[in] names Separated test case names
 * \param[in] flag Enabled flag
 * \return The number of tests updated
 * \sa TDOG_SKIP_TEST()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SET_ENABLED(names, flag)
#else
  // Actual
  #define TDOG_SET_ENABLED(names, flag) \
  tdog::runner::global().set_enabled(names, flag)
#endif

/**
 * \brief Allows a test to be skipped with a statement from within the
 * test implementation code.
 * \details This macro terminates the test execution and puts the test
 * into the "skipped" state. Any test results are cleared.
 *
 * This can be used by the test author to skip the test. Skipping a test effectively
 * disables it, but the skipped and disabled states are considered disctinct.
 * If used, the skip macro should be placed at or near the start of the test code,
 * as execution will occur up to the point it is called.
 *
 * Example:
 *
 * \code
 * TDOG_TEST_CASE(some_test)
 * {
 *   TDOG_SET_AUTHOR("Employee 24601");
 *
 *   // Disable this test for the time being
 *   TDOG_SKIP_TEST();
 *
 *   // Unexecuted statements
 *   ...
 * }
 * \endcode
 *
 * Note. This macro can only be used within the scope of a test case.
 * \sa TDOG_SET_ENABLED()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_SKIP_TEST()
#else
  // Actual
  #define TDOG_SKIP_TEST() \
  tdog_helper->set_skipped(); return
#endif

/**
 * @}
 * @name Miscellaneous macros
 * @{
 */

/**
 * \brief Make the test sleep for the specified (minimum) number of
 * milliseconds.
 * \details Accuracy is platform dependent.
 *
 * Note. This macro can only be used within the scope of a test case.
 * \param[in] ms Integer number of milliseconds
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_TEST_SLEEP(ms)
#else
  // Actual
  #define TDOG_TEST_SLEEP(ms) \
  tdog_helper->sleep(ms)
#endif

/**
 * \brief Allows direct access to the underlying implementation instance of
 * a test case.
 * \details When you declare a test case with TDOG_TEST_CASE() or TDOG_TEST_FIXTURE(),
 * for example, a C++ class is instantiated and your assert statements will
 * form part of the class implementation. You would not normally need to access
 * the underlying class instance directly, however, as these objects "self-register"
 * with a global tdog::runner object which provides the means to run the
 * tests. However, if you wish to register test cases explicitly with the runner
 * yourself (rather than have them register themselves), then you will need
 * a mechanism by which to access to the underlying test instance, and
 * the TDOG_GET_TCPTR() macro provides this. The TDOG library uses this
 * in order to test itself, but it is not generally needed by client
 * applications.
 *
 * Below is an example of how to register a test case explicitly with the
 * runner using this macro. Note that in order to make tests non self-registering,
 * you must declare them inside the scope of TDOG_EXPLICIT_SUITE().
 *
 * \code
 * #include <tdog.hpp>
 *
 * TDOG_EXPLICIT_SUITE(suite_name)
 * {
 *   // Define test
 *   TDOG_TEST_CASE(some_test)
 *   {
 *     ...
 *   }
 *
 *   // Must close the suite
 *   TDOG_CLOSE_SUITE
 * }
 *
 *
 * int main(int argc, char **argv)
 * {
 *   // Explicitly register with non-global runner
 *   tdog::runner tr;
 *   tr.register_test(TDOG_GET_TCPTR(suite_name::some_test));
 *
 *   // Run the test
 *   return tr.run("*");
 * }
 * \endcode
 *
 * Tests within sub-suites of the TDOG_EXPLICIT_SUITE() will also
 * require explicit registration with a runner, irrespective of
 * whether the sub-suite is declared using TDOG_SUITE() or
 * TDOG_EXPLICIT_SUITE(). Tests in the global suite namespace cannot
 * be made "explicit", and will always self-register.
 *
 * There will be a compile time error if the test name does not exist.
 * Otherwise, the result is a pointer of type tdog::basic_test. The test case
 * instance must exist for the lifetime of the runner class.
 * \param[in] name Test case name
 * \return Pointer to underlying test object.
 * \sa tdog::runner::register_test()
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  // Doc dummy
  #define TDOG_GET_TCPTR(name)
#else
  // Actual
  #define TDOG_GET_TCPTR(test_name) \
  &test_name##_inst
#endif

/**
 * \brief Macro used internally to ensure test suite namespaces are unique when
 * declared across multiple header files.
 * \details This macro is defined as "__COUNTER__", a non-standard but widely
 * supported macro which provides an incrementing value each time it is used.
 * It is used internally to ensure that discontiguous test suites, i.e.
 * test suites declarations with the same fully qualified name in multiple
 * include files, can be used without conflict*.
 *
 * For example, without the use of the counter, the following will give rise to
 * a suite naming conflict:
 *
 * \code
 * // test_file1.hpp
 * TDOG_SUITE(project_suite)
 * {
 *  TDOG_TEST_CASE(some_test1)
 *  {
 *    ...
 *  }
 *
 *  // Must close suite
 *  TDOG_CLOSE_SUITE
 * }
 * \endcode
 *
 * \code
 * // test_file2.hpp
 * TDOG_SUITE(project_suite)
 * {
 *  TDOG_TEST_CASE(some_test2)
 *  {
 *    ...
 *  }
 *
 *  // Must close suite
 *  TDOG_CLOSE_SUITE
 * }
 * \endcode
 *
 * If "__COUNTER__" is not supported on your compiler, you should see a compilation
 * warning of this. In this case, it's not a big restriction, but you must ensure that
 * each fully qualified suite name is unique in each test include file. In above code,
 * for example, the test suite should be given a different name in "test_file2.hpp".
 * Pre-defining TDOG_COUNTER_ID as an empty value will silence the compiler warning when
 * building your project.
 *
 * *The conflict would arise from the internal machinary needed to track the namespace,
 * rather than the namespace itself.
 */
#if defined(DOXYGEN)
  #define TDOG_COUNTER_ID __COUNTER__
#else
  #if !defined(TDOG_COUNTER_ID)
    #if defined(__COUNTER__)
      #define TDOG_COUNTER_ID __COUNTER__
    #else
      #if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__TURBOC__)
        #pragma message ("warning: The __COUNTER__ macro is not supported on your compiler. \
This is needed to support discontiguous test suites (i.e. suites of the same name shared across \
multiple files). See TDOG_COUNTER_ID macro for information.")
      #else
        #warning The __COUNTER__ macro is not supported on your compiler. \
This is needed to support discontiguous test suites (i.e. suites of the same name shared across \
multiple files). See TDOG_COUNTER_ID macro for information.
      #endif
    #endif // __COUNTER__
  #endif // TDOG_COUNTER_ID
#endif // DOXYGEN

/**
 * \brief Build platform OS name macro.
 * \details This is defined automatically, but can be overridden if pre-defined to a
 * custom string value.
 */
#if defined(DOXYGEN) && defined(DOXYGEN_HIDEIMPL)
  #define TDOG_PLATOS <platform os>
#else
  #if !defined(TDOG_PLATOS)

    #if defined(WIN64) || defined(_WIN64) || defined(__x86_64) || defined(__x86_64__) || \
      defined(_M_X64) || __SIZEOF_SIZE_T__ == 8
      #define TDOG_BUS_SIZE " 64-bit"
    #elif defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_IX86) || \
      defined(_X86_) || __SIZEOF_SIZE_T__ == 4
      #define TDOG_BUS_SIZE " 32-bit"
    #endif // WIN64

    #if defined(_MSC_VER) || defined(WINDOWS) || defined(_WINDOWS) || \
      defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64)
      #define TDOG_PLATOS "WINDOWS" TDOG_BUS_SIZE
    #elif defined(__hpux)
      #define TDOG_PLATOS "HP-UX" TDOG_BUS_SIZE
    #elif defined(_AIX)
      #define TDOG_PLATOS "AIX" TDOG_BUS_SIZE
    #elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || \
      defined(__DragonFly__)
      #define TDOG_PLATOS "BSD" TDOG_BUS_SIZE
    #elif defined(sun) || defined(__sun)
      #define TDOG_PLATOS "SOLARIS" TDOG_BUS_SIZE
    #elif defined(__APPLE__) || defined(__MAC__)
      #define TDOG_PLATOS "APPLE" TDOG_BUS_SIZE
    #elif defined(__linux) || defined(__linux__)
      #define TDOG_PLATOS "LINUX" TDOG_BUS_SIZE
    #elif defined(__unix) || defined(__unix__)
      #define TDOG_PLATOS "UNIX" TDOG_BUS_SIZE
    #else
      #define TDOG_PLATOS "UNKNOWN"
    #endif

  #endif // TDOG_PLATOS
#endif // DOXYGEN

/**
 * @}
 */

//---------------------------------------------------------------------------
#endif // HEADER GUARD
//---------------------------------------------------------------------------
