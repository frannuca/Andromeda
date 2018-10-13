
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

BOOST_AUTO_TEST_CASE(YIELDBOOTSTRAP)
{
	std::cout << "Test yield curve bootstrap" << std::endl;
	yield::YieldCurveBuilder ybuilder(qtime::QDate(01, 01, 2018));
	std::unique_ptr<qtime::DayCounter> dc(new qtime::Actual365fixed());
	qtime::QDate T0(01, 01, 2018);
	instrument::CashFlow flow1(T0, 3_months, 1e6);
	instrument::CashFlow flow2(T0, 6_months, 1e6);

	std::unique_ptr<instrument::xIbor> libor1m(new instrument::xIbor(T0, 0.01, 1_months));
	std::unique_ptr<instrument::xIbor>  libor3m(new instrument::xIbor(T0, 0.015, 3_months));
	std::unique_ptr<instrument::xIbor>  libor6m(new instrument::xIbor(T0, 0.019, 6_months));
	std::unique_ptr<instrument::xIbor>  libor9m(new instrument::xIbor(T0, 0.0197, 9_months));
	std::unique_ptr<instrument::xIbor>  libor12m(new instrument::xIbor(T0, 0.02, 12_months));
	std::unique_ptr<instrument::Swap> swap1(new instrument::Swap(qtime::QDate(01, 01, 2020), 0.04, flow1, flow2));
	std::unique_ptr<instrument::Swap>  swap3(new instrument::Swap(qtime::QDate(01, 01, 2021), 0.05, flow1, flow2));
	std::unique_ptr<instrument::Swap>  swap5(new instrument::Swap(qtime::QDate(01, 01, 2023), 0.06, flow1, flow2));
	std::unique_ptr<instrument::Swap>  swap10(new instrument::Swap(qtime::QDate(01, 01, 2028), 0.1, flow1, flow2));
	std::unique_ptr <yield::YieldCurve> oyield = ybuilder.withDayCount(dc.get())
		.withInstrument(swap1.get())
		.withInstrument(swap3.get()).withInstrument(swap5.get()).withInstrument(swap10.get()).withInstrument(libor1m.get()).withInstrument(libor3m.get())
		.withInstrument(libor6m.get()).withInstrument(libor9m.get()).withInstrument(libor12m.get())
		.Build();

	oyield->boostrap();

	int N = 10;
	for (auto n : boost::irange(0, N, 1))
	{
		double t = 10.0 * n / double(N);
		std::cout << "t=" << t << " r=" << oyield->rate(Tenor<SYEAR>(t)) << std::endl;
	}

	auto f11y = oyield->forward(T0 + 365, 1_years);
	auto f16m = oyield->forward(T0 + 365, 6_months);
	auto f13m = oyield->forward(T0 + 365, 3_months);
	auto f11d = oyield->forward(T0 + 365, 1_days);
	auto d1 =   oyield->discount(1_years);
	auto zz = 1 / (1 + f11d * to_years(Tenor<SDAY>(1)));
}
