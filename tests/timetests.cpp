
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE differenciationTest
#include <cmath>
#include <boost/test/included/unit_test.hpp> // this work

#include <iostream>
#include <memory>
#include "../src/qtime/tenor.h"
#include "../src/yield/YieldCurve.h"
#include <boost/range/counting_range.hpp>
#include <boost/range/irange.hpp>

using namespace qtime;
using namespace literals;

namespace tt = boost::test_tools;

BOOST_AUTO_TEST_CASE(QTIME)
{	
	std::cout << "Test Tenor conversions" << std::endl;
	Tenor<SMONTH> t12monthfromyears = -1.0 * 1_years;
	Tenor<SMONTH> t48monthfromyears = 4_years;
	Tenor<SDAY> t365daysfrom1years = 1_years;
	Tenor<SDAY> t3x365daysfrom3years = 3_years;
	
	BOOST_CHECK_MESSAGE(t12monthfromyears == -1.0 * 12_months, "Invalid conversion years to month (12 months)");
	BOOST_CHECK_MESSAGE(t48monthfromyears == 4* 12_months, "Invalid conversion years to month (4*12months)");
	BOOST_CHECK_MESSAGE(t365daysfrom1years == 360_days, "Invalid conversion years to month (365 days)");
	BOOST_CHECK_MESSAGE(t3x365daysfrom3years == 3 * 360_days, "Invalid conversion years to month (3 * 365 days)");

	Tenor<SMONTH> t48_12 = t48monthfromyears - 12_months;
	BOOST_CHECK_MESSAGE(t48_12 == 36_months, "Invalid conversion years to month");

	std::cout << "Testing Tenor operations" << std::endl;
	BOOST_CHECK_MESSAGE(1_days + 3_days == 4_days, "Error: 1_days + 3_days <> 4_days");
	BOOST_CHECK_MESSAGE(1_weeks + 3_weeks == 4_weeks, "Error: 1_weeks + 3_weeks <> 4_weeks");
	BOOST_CHECK_MESSAGE(1_months + 3_months == 4_months, "Error: 1_months + 3_months <> 4_months");
	BOOST_CHECK_MESSAGE(3 * 3_months == 9_months, "Error: 3 * 3_months <> 9_months");
	BOOST_CHECK_MESSAGE(3_months * 4 == 12_months, "Error: 3_months * 4 <> 12_months");
	
}
/**
 * yield::YieldCurveBuilder ybuilder(qtime::QDate(01,01,2018));
	std::unique_ptr<qtime::DayCounter> dc(new qtime::Actual365fixed());
	qtime::QDate T0(01, 01, 2018);
	instrument::CashFlow flow1(T0, 3_months,1e6);
	instrument::CashFlow flow2(T0, 6_months, 1e6);
	
	std::unique_ptr<instrument::xIbor> libor1m(new instrument::xIbor(T0,0.01, 1_months));
	std::unique_ptr<instrument::xIbor>  libor3m(new instrument::xIbor(T0, 0.015, 3_months));
	std::unique_ptr<instrument::xIbor>  libor6m(new instrument::xIbor(T0, 0.019, 6_months));
	std::unique_ptr<instrument::xIbor>  libor9m(new instrument::xIbor(T0, 0.0197, 9_months));
	std::unique_ptr<instrument::xIbor>  libor12m(new instrument::xIbor(T0, 0.02, 12_months));
	std::unique_ptr<instrument::Swap> swap1(new instrument::Swap(qtime::QDate(01, 01, 2020),0.04,flow1,flow2));
	std::unique_ptr<instrument::Swap>  swap3(new instrument::Swap(qtime::QDate(01, 01, 2021), 0.05, flow1, flow2));
	std::unique_ptr<instrument::Swap>  swap5 (new instrument::Swap(qtime::QDate(01, 01, 2023), 0.06, flow1, flow2));
	std::unique_ptr<instrument::Swap>  swap10(new instrument::Swap(qtime::QDate(01, 01, 2028), 0.1, flow1, flow2));
	std::unique_ptr <yield::YieldCurve> oyield = ybuilder.withDayCount(std::move(dc))
									  .withInstrument(swap1.get())
									  .withInstrument(swap3.get()).withInstrument(swap5.get()).withInstrument(swap10.get()).withInstrument(libor1m.get()).withInstrument(libor3m.get())
									  .withInstrument(libor6m.get()).withInstrument(libor9m.get()).withInstrument(libor12m.get())
									  .Build();

	oyield->boostrap();

	int N = 10;
	for(auto n: boost::irange(0,N,1))
	{
		double t = 10.0 * n / double(N);
		std::cout << "t="<<t<<" r=" << oyield->rate(Tenor<SYEAR>(t)) << std::endl;
	}

	auto f11y = oyield->forward(T0 + 365, qtime::Tenor<SDAY>(365));
	auto f16m = oyield->forward(T0 + 365, qtime::Tenor<SDAY>(30 * 6));
	auto f13m = oyield->forward(T0 + 365, qtime::Tenor<SDAY>(30 * 3));
	auto f11d = oyield->forward(T0 + 365, qtime::Tenor<SDAY>(1));
	auto d1 = oyield->discount(1_years);
	auto zz = 1 / (1 + f11d * to_years(Tenor<SDAY>(1)));
 */
