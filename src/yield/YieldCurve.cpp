#include "YieldCurve.h"
#include <boost/range/combine.hpp>
#include "../qtime/schedule.h"
#include "../qtime/GermanCalendar.h"
#include "../qtime/buinessdayconvention.h"
#include <vector>
#include <numeric>

namespace
{
	boost::optional<std::pair<double, double>> None;
}

yield::YieldCurve::YieldCurve(const qtime::DayCounter* dc, const qtime::Calendar* calendar, const qtime::QDate& t0, std::vector<const instrument::Instrument*> pintr):
t0_(t0),pinstruments(pintr),calendar_(calendar),dc_(dc)
{
}

double yield::YieldCurve::discount(qtime::Tenor<qtime::SDAY> ndays)
{
	double t = qtime::to_years(ndays);
	return compute_discount(t);
	
}

double yield::YieldCurve::discount(qtime::Tenor<qtime::SYEAR> nyears)
{
	double t = nyears.n;
	return compute_discount(t);
}

double yield::YieldCurve::discount(qtime::Tenor<qtime::SWEEK> nweeks)
{
	double t = qtime::to_years(nweeks);
	return compute_discount(t);
}

double yield::YieldCurve::discount(qtime::Tenor<qtime::SMONTH> nmonths)
{
	double t = qtime::to_years(nmonths);
	return compute_discount(t);
}

double yield::YieldCurve::discount(const qtime::QDate& t)
{
	qtime::Tenor<qtime::SDAY> day(t-t0_);
	return compute_discount(qtime::to_years(day));
}

double yield::YieldCurve::compute_rate(const double &years)
{
	return interpolated_rate(years, None);
}

double yield::YieldCurve::compute_discount(const double& t)
{
	double r = interpolated_rate(t, None);
	return std::exp(-r * t);
}

double yield::YieldCurve::rate(qtime::Tenor<qtime::SDAY> ndays)
{	
	return compute_rate(qtime::to_years(ndays));
}

double yield::YieldCurve::rate(qtime::Tenor<qtime::SYEAR> nyears)
{
	return compute_rate(qtime::to_years(nyears));
}

double yield::YieldCurve::rate(qtime::Tenor<qtime::SWEEK> nweeks)
{
	return compute_rate(qtime::to_years(nweeks));
}

double yield::YieldCurve::rate(qtime::Tenor<qtime::SMONTH> nmonths)
{
	return compute_rate(qtime::to_years(nmonths));
}

double yield::YieldCurve::rate(const qtime::QDate& t)
{
	qtime::Tenor<qtime::SDAY> dt(t - t0_);
	return compute_rate(qtime::to_years(dt));
}

double yield::YieldCurve::forward(const qtime::QDate& td1, qtime::Tenor<qtime::SDAY> ndays)
{
	auto t1 = dc_->yearfraction(t0_, td1);		
	auto dt2 = qtime::to_years(ndays);
	auto t2 = t1 + dt2;
	
	return log(exp(compute_rate(t2)*t2 - compute_rate(t1)*t1)) / dt2;
	/*1 / (1 + f * dt)*exp(-r1 * t1) = exp(-r2 * t2)
		1 +exp( f * dt) = exp(r2*t2 - r1 * t1)
		f *dt = ln(exp(r2*t2 - r1 * t1) - 1);*/
}


double yield::YieldCurve::forward(const qtime::QDate& t1, const qtime::QDate& t2)
{
	throw "Not Implemented";
}

yield::YieldCurveBuilder::YieldCurveBuilder(const qtime::QDate& t0):t0_(t0)
{}


const yield::YieldCurveBuilder& yield::YieldCurveBuilder::withInstrument(const instrument::Instrument* pintr) const
{
	instruments.push_back(pintr);
	return *this;
}

const yield::YieldCurveBuilder& yield::YieldCurveBuilder::withDayCount(const qtime::DayCounter* pdc) const
{
	dc = std::move(pdc);
	return *this;
}

const yield::YieldCurveBuilder& yield::YieldCurveBuilder::withCalendar(const qtime::Calendar* pcalendar) const
{
	this->calendar = pcalendar;
	return *this;
}

std::unique_ptr<yield::YieldCurve> yield::YieldCurveBuilder::Build() const
{
	if(!dc)
	{
		dc.reset(new qtime::SimpleDayCounter());
	}
	if(!calendar)
	{
		calendar.reset(new qtime::GermanCalendar());
	}
	return std::move(std::unique_ptr<yield::YieldCurve>(new YieldCurve(*dc,*calendar, t0_,instruments)));
}

void yield::YieldCurve::boostrap()
{
	using namespace instrument;
	std::vector<const instrument::Instrument*> swaps,xibors;
	std::copy_if(pinstruments.begin(), pinstruments.end(), std::back_inserter(swaps), [](auto p) {return dynamic_cast<const instrument::Swap*>(p) != nullptr; });
	std::copy_if(pinstruments.begin(), pinstruments.end(), std::back_inserter(xibors), [](auto p) {return dynamic_cast<const instrument::xIbor*>(p) != nullptr; });

	std::sort(xibors.begin(),xibors.end(),[](auto a, auto b){
		const auto * x = dynamic_cast<const instrument::xIbor*>(a);
		const auto * y = dynamic_cast<const instrument::xIbor*>(b);
		return qtime::to_years(x->tenor) < qtime::to_years(y->tenor);
		
	});

	std::sort(swaps.begin(), swaps.end(), [](auto a, auto b)
	{
		const auto * x = dynamic_cast<const instrument::Swap*>(a);
		const auto * y = dynamic_cast<const instrument::Swap*>(b);
		return x->maturity < y->maturity;
	});

	for (auto pinstr : xibors)
	{
		Instrument* pp = const_cast<Instrument*>(pinstr);		
		xIbor* libor = dynamic_cast<instrument::xIbor*>(pp);
		if(libor != nullptr)
		{
			auto c = libor->C;
			this->t_.push_back(qtime::to_years(libor->tenor));
			this->rates_.push_back(libor->C);
		}
	}

	
	for(auto swap: swaps)
	{
		const Swap* pswap = static_cast<const instrument::Swap*>(swap);		
		auto tmat = int(to_years(qtime::Tenor<qtime::SDAY>(pswap->maturity - t0_)));
		double S = pswap->C;
		double l = 0, r = 1;
		double m = (l + r) / 2.0;
		double Scalc = priceSwap(pswap, m);

		while(std::abs(Scalc-S)>1e-3)
		{
			if (priceSwap(pswap, m) < S)
			{
				r = m;
			}
			else
			{
				l = m;
			}

			m = (r + l)*0.5;
			Scalc = priceSwap(pswap, m);
		}
		
		t_.push_back(tmat);
		rates_.push_back(-log(m)/double(tmat));
		
	}
}


double yield::YieldCurve::interpolated_rate(const double& t, boost::optional<std::pair<double, double>> pn)
{
	double t0, t1, r0, r1;

	if(t > t_.back()){
		if(pn)
		{
			t0 = t_.back();
			r0 = rates_.back();
			t1 = pn.value().first;
			r1 = pn.value().second;
		}
		else
		{
			throw "time is out the range and no optional rate in the future is provided";
		}
	}
	else
	{
		std::vector<double>::iterator low, up;
		low = std::lower_bound(t_.begin(), t_.end(), t);		
		up = std::upper_bound(t_.begin(), t_.end(), t);
		
		if(low != t_.begin() && *low > t)
		{
			auto a = --low;
			low = a;
		}

		int i = std::distance(t_.begin(), low);
		int j = std::distance(t_.begin(), up);
		
		i = i < 0 ? 0 : i;

		if(up == t_.end())
		{
			j = i;
		}
		else if(low == t_.end())
		{
			i = j;
		}

		t0 = t_[i];
		t1 = t_[j];
		r0 = rates_[i];
		r1 = rates_[j];

		if (std::abs(t1 - t0) < 1e-12)
		{
			return rates_[i];
		}
	}
	
		
	return (r1 - r0) / (t1 - t0)*(t - t0) + r0;
		
}

double yield::YieldCurve::priceSwap(const instrument::Swap* swap,const double &p)
{
	//computing time in years:
	
	qtime::BusinessDayConvention c1(calendar_, qtime::BusinessDayConvention::TYPE::ModifiedFollowing);
	qtime::BusinessDayConvention c2(calendar_, qtime::BusinessDayConvention::TYPE::ModifiedFollowing);
	//QDate effectiveDate_, const QDate& terminationDate_, const Tenor<SMONTH> tenor_, const qtime::Calendar* calendar_, const BusinessDayConvention* convention_, const BusinessDayConvention* terminationDateConvention_, DateGeneration::Rule rule_
	
	qtime::Schedule schedule(t0_, swap->maturity, swap->floating_leg->tenor, calendar_, &c1, &c2,qtime::DateGeneration::Rule::Backward);
	
	std::vector<double> periods;
	auto dates = schedule.Dates();
	for(int i=1;i<dates.size();++i)
	{
		periods.push_back(dc_->yearfraction(dates[i-1], dates[i]));
		std::cout << periods.back() << std::endl;
	}

	double tmat = std::accumulate(periods.begin(), periods.end(), 0.0, [](const double& acc, const double& z) {return acc + z; });
	
	double r = -log(p) / tmat;
	double num = 1-p;
	double den = 0;
	
	double taccum = 0;
	for(auto dt: periods)
	{
		taccum += dt;
		boost::optional<std::pair<double, double>> xp(std::make_pair(tmat, r));
		auto rx = interpolated_rate(taccum, xp);
		den += std::exp(-rx * taccum)*dt;		
	}

	return num / den;
}



