/**
 * @file   DetectorTimingTypes_test.cc
 * @brief  Test of `detinfo::DetectorTimings` with `DetectorClocksStandard`.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   June 27, 2019
 */

// Boost libraries
#define BOOST_TEST_MODULE ( DetectorTimingTypes_test )
#include <boost/test/unit_test.hpp>

// LArSoft libraries
#include "lardataalg/DetectorInfo/DetectorTimingTypes.h"
#include "lardataalg/Utilities/quantities/electronics.h"
#include "lardataalg/Utilities/quantities/spacetime.h"
#include "larcorealg/CoreUtils/DebugUtils.h" // lar::debug::::static_assert_on<>
#include "larcorealg/CoreUtils/MetaUtils.h" // util::is_same_decay_v
#include "test/Utilities/disable_boost_fpc_tolerance.hpp"

// C/C++ standard libraries
#include <type_traits> // std::decay_t<>, std::is_same_v<>


//------------------------------------------------------------------------------
//--- static tests
//------------------------------------------------------------------------------

static_assert(!detinfo::timescales::is_tick_v<double>);
static_assert
  (!detinfo::timescales::is_tick_v<util::quantities::second>);
static_assert
  ( detinfo::timescales::is_tick_v<util::quantities::tick>);
static_assert
  ( detinfo::timescales::is_tick_v<util::quantities::tick_d>);
static_assert
  (!detinfo::timescales::is_tick_v<detinfo::timescales::optical_time>);
static_assert
  ( detinfo::timescales::is_tick_v<detinfo::timescales::optical_tick>);
static_assert
  ( detinfo::timescales::is_tick_v<detinfo::timescales::optical_time_ticks>);

//------------------------------------------------------------------------------
static_assert(
  detinfo::timescales::timescale_traits<detinfo::timescales::optical_time>
    ::same_category_as<detinfo::timescales::optical_time>
  );
static_assert(
  detinfo::timescales::timescale_traits<detinfo::timescales::optical_time>
    ::category_compatible_with<detinfo::timescales::optical_time>
  );
static_assert(
  detinfo::timescales::timescale_traits<detinfo::timescales::optical_time>
    ::same_category_as<detinfo::timescales::optical_tick>
  );
static_assert(
  detinfo::timescales::timescale_traits<detinfo::timescales::optical_time>
    ::category_compatible_with<detinfo::timescales::optical_tick>
  );


//------------------------------------------------------------------------------
static_assert(
  detinfo::timescales::optical_time::category_compatible_with
    <util::quantities::NoCategory>()
  );
static_assert(
  detinfo::timescales::optical_time::category_compatible_with
    <detinfo::timescales::optical_time>()
  );
static_assert(
  detinfo::timescales::optical_time::category_compatible_with
    <detinfo::timescales::time_interval>()
  );
static_assert(
  detinfo::timescales::optical_time::category_compatible_with
    <detinfo::timescales::optical_tick>()
  );
static_assert(
  detinfo::timescales::optical_time::category_compatible_with
    <detinfo::timescales::optical_tick_d>()
  );
static_assert(
  detinfo::timescales::optical_time::category_compatible_with
    <detinfo::timescales::optical_time_ticks>()
  );
static_assert(
  detinfo::timescales::optical_time::category_compatible_with
    <detinfo::timescales::optical_time_ticks_d>()
  );
static_assert(
  !detinfo::timescales::optical_time::category_compatible_with
    <detinfo::timescales::electronics_time>()
  );


// -----------------------------------------------------------------------------
// optical type static tests
// -----------------------------------------------------------------------------
static_assert(std::is_same_v<
  detinfo::timescales::optical_tick,
  detinfo::timescales::timescale_traits
    <detinfo::timescales::OpticalTimeCategory>::tick_t
  >);
static_assert(std::is_same_v<
  detinfo::timescales::optical_time_ticks,
  detinfo::timescales::timescale_traits
    <detinfo::timescales::OpticalTimeCategory>::tick_interval_t
  >);


// -----------------------------------------------------------------------------
void test_time_operations()
{
  using namespace util::quantities::time_literals;
  using namespace detinfo::timescales;

  using traits_t = timescale_traits<OpticalTimeCategory>;

  BOOST_TEST_MESSAGE("Testing category: " << traits_t::name());

  using time_point_t    = traits_t::time_point_t;
  using time_interval_t = traits_t::time_interval_t;

  //
  // time points and intervals
  //
  time_point_t p  { 10.0_us };
  time_point_t p2 {  5.0_us };

  time_interval_t dt { 500_ns };

  static_assert(util::is_same_decay_v<decltype(p + dt), time_point_t>);
  BOOST_TEST(p + dt == 10.5_us);
  static_assert(util::is_same_decay_v<decltype(p + 500_ns), time_point_t>);
  BOOST_TEST(p + 500_ns == 10.5_us);

  static_assert(util::is_same_decay_v<decltype(p - dt), time_point_t>);
  BOOST_TEST(p - dt == 9.5_us);
  static_assert(util::is_same_decay_v<decltype(p - 500_ns), time_point_t>);
  BOOST_TEST(p - 500_ns ==  9.5_us);

  static_assert(util::is_same_decay_v<decltype(p - p2), time_interval_t>);
  BOOST_TEST(p - p2 == 5.0_us);
} // test_time_operations()


// -----------------------------------------------------------------------------
void test_integral_tick_operations()
{
  using namespace util::quantities::electronics_literals;
  using namespace detinfo::timescales;

  using traits_t = timescale_traits<OpticalTimeCategory>;

  BOOST_TEST_MESSAGE("Testing category: " << traits_t::name());

  using tick_t            = traits_t::tick_t;
  using tick_interval_t   = traits_t::tick_interval_t;

  //
  // ticks
  //
  tick_t tick  { 100.0_tick };
  tick_t tick2 {  50.0_tick };

  tick_interval_t dtick { 30_tick };

  static_assert(util::is_same_decay_v<decltype(tick + dtick), tick_t>);
  BOOST_TEST(tick + dtick == 130.0_tick);
  static_assert(util::is_same_decay_v<decltype(tick + 30.0_tick), tick_t>);
  BOOST_TEST(tick + 30.0_tick == 130.0_tick);

  static_assert(util::is_same_decay_v<decltype(tick - dtick), tick_t>);
  BOOST_TEST(tick - dtick == 70.0_tick);
  static_assert(util::is_same_decay_v<decltype(tick - 30.0_tick), tick_t>);
  BOOST_TEST(tick - 30.0_tick == 70.0_tick);

  static_assert(util::is_same_decay_v<decltype(tick - tick2), tick_interval_t>);
  BOOST_TEST(tick - tick2 == 50_tick);
} // test_integral_tick_operations()


// -----------------------------------------------------------------------------
void test_real_tick_operations()
{
  using namespace util::quantities::electronics_literals;
  using namespace detinfo::timescales;
  using traits_t = timescale_traits<OpticalTimeCategory>;

  BOOST_TEST_MESSAGE("Testing category: " << traits_t::name());

  using tick_d_t          = traits_t::tick_d_t;
  using tick_interval_d_t = traits_t::tick_interval_d_t;

  //
  // ticks
  //
  tick_d_t tick  { 100.5_tickd };
  tick_d_t tick2 {  50.0_tickd };

  tick_interval_d_t dtick { 30_tickd };

  static_assert(util::is_same_decay_v<decltype(tick + dtick), tick_d_t>);
  BOOST_TEST(tick + dtick == 130.5_tickd);
  static_assert(util::is_same_decay_v<decltype(tick + 30.0_tickd), tick_d_t>);
  BOOST_TEST(tick + 30.0_tickd == 130.5_tickd);

  static_assert(util::is_same_decay_v<decltype(tick - dtick), tick_d_t>);
  BOOST_TEST(tick - dtick == 70.5_tickd);
  static_assert(util::is_same_decay_v<decltype(tick - 30.0_tickd), tick_d_t>);
  BOOST_TEST(tick - 30.0_tickd == 70.5_tickd);

  static_assert
    (util::is_same_decay_v<decltype(tick - tick2), tick_interval_d_t>);
  BOOST_TEST(tick - tick2 == 50.5_tickd);


  /// Type of a point in time, measured in ticks.
  using ClockTick_t = optical_tick;

  /// Type of a time interval, measured in ticks.
  using ClockTicks_t = optical_time_ticks;

  ClockTick_t mytick { 10_tick };
  ClockTicks_t mydelay { 5_tick };

  ClockTick_t delayedTick = mytick + mydelay;
  BOOST_TEST(delayedTick == 15_tick);
} // test_real_tick_operations()

// ---------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(DetectorTimingTypes_test)

BOOST_AUTO_TEST_CASE(OpticalTime_testcase)
{
  test_time_operations();
  test_integral_tick_operations();
  test_real_tick_operations();
}

BOOST_AUTO_TEST_SUITE_END()
