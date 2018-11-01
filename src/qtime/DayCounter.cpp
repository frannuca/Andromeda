#include "DayCounter.h"
#include <sstream>
#include <cmath>
#include <algorithm>

namespace qtime
{
	DayCounter::DayCounter()
	{
	}

	Actual360::Actual360(bool includelastday) :_includeLastDay(includelastday)
	{
	}

	std::string Actual360::name()
	{
		return _includeLastDay ? "Actual360 (inc)" : "Actual360";
	}

	double Actual360::dayCount(const QDate& d1, const QDate& d2) const
	{		
		auto dd = (d2 - d1);
		return dd + (_includeLastDay ? 1.0 : 0.0);
	}

	double Actual360::yearfraction(const QDate& d1, const QDate& d2) const
	{
		return dayCount(d1,d2) / 360.0;
	}

	double Actual360::daysperyear() const
	{
		return 360;
	}

	Actual365fixed::Actual365fixed()
	{
	}

	std::string Actual365fixed::name()
	{
		return "Actual365fixed";
	}

	double Actual365fixed::dayCount(const QDate& d1, const QDate& d2) const
	{
		static const int MonthOffset[] = {

			0,  31,  59,  90, 120, 151,  // Jan - Jun

			181, 212, 243, 273, 304, 334   // Jun - Dec

		};



		int s1 = d1.DayOfTheMonth()+ MonthOffset[int(d1.Month()) - 1] + (d1.Year() * 365);

		int s2 = d2.DayOfTheMonth() + MonthOffset[int(d2.Month()) - 1] + (d2.Year() * 365);



		if (d1.Month() == MONTH::FEB && d1.DayOfTheMonth() == 29) {
			--s1;
		}



		if (d2.Month() == MONTH::FEB && d2.DayOfTheMonth() == 29) {
			--s2;
		}



		return s2 - s1;
	}

	double Actual365fixed::yearfraction(const QDate& d1, const QDate& d2) const
	{
		return dayCount(d1, d2) / 365.0;		
	}

	double Actual365fixed::daysperyear() const
	{
		return 365;
	}

	Thirty360::Thirty360(CONVENTION c, bool isLastPeriod):_convention(c),_isLastPeriod(isLastPeriod)
	{
		switch (_convention)
		{
		case(CONVENTION::BONDBASIS):
		case(CONVENTION::USA): 
			this->daycount_functor = [](const QDate& d1, const QDate& d2)
			{
					int dd1 = d1.DayOfTheMonth(), dd2 = d2.DayOfTheMonth();
					int mm1 = int(d1.Month());
					int mm2 = int(d2.Month());
					int yy1 = d1.Year(), yy2 = d2.Year();
					if (dd2 == 31 && dd1 < 30) { dd2 = 1; mm2++; }
					return 360 * (yy2 - yy1) + 30 * (mm2 - mm1 - 1) +

						std::max(0, 30 - dd1) + std::min(30, dd2);
			};
			break;
		case(CONVENTION::EUROBONDBASIS):
		case(CONVENTION::EUROPEAN):
			this->daycount_functor = [](const QDate& d1, const QDate& d2)
			{
				int dd1 = d1.DayOfTheMonth(), dd2 = d2.DayOfTheMonth();
				int mm1 = int(d1.Month()), mm2 = int(d2.Month());
				int yy1 = d1.Year(), yy2 = d2.Year();
				return 360 * (yy2 - yy1) + 30 * (mm2 - mm1 - 1) +
					std::max(0, 30 - dd1) + std::min(30, dd2);
			};
			break;		
		case(CONVENTION::ITALIAN): this->daycount_functor =
			[](const QDate& d1, const QDate& d2)
			{
			int dd1 = d1.DayOfTheMonth(), dd2 = d2.DayOfTheMonth();
			int mm1 = int(d1.Month()), mm2 = int(d2.Month());
			int yy1 = d1.Year(), yy2 = d2.Year();
			if (mm1 == 2 && dd1 > 27) dd1 = 30;
			if (mm2 == 2 && dd2 > 27) dd2 = 30;
			return 360 * (yy2 - yy1) + 30 * (mm2 - mm1 - 1) +
				std::max(0, 30 - dd1) + std::min(30, dd2);
			};
			break;
		case(CONVENTION::GERMAN): this->daycount_functor =
			[this](const QDate& d1, const QDate& d2)
			{
				int dd1 = d1.DayOfTheMonth(), dd2 = d2.DayOfTheMonth();

				int mm1 = int(d1.Month()), mm2 = int(d2.Month());

				int yy1 = d1.Year(), yy2 = d2.Year();

				if (mm1 == 2 && dd1 == 28 + (QDate::isLeapYear(yy1) ? 1 : 0))
					dd1 = 30;
				if (!_isLastPeriod && mm2 == 2 &&
					dd2 == 28 + (QDate::isLeapYear(yy2) ? 1 : 0))
					dd1 = 30;
				return 360 * (yy2 - yy1) + 30 * (mm2 - mm1 - 1) +
					std::max(0, 30 - dd1) + std::min(30, dd2);
			};
			break;
		default:
			throw "Unknown convention type";
		}

	}

	

	std::string Thirty360::name()
	{
		std::ostringstream oss;
		oss << "30/360-";

		switch (_convention)
		{
		case(CONVENTION::USA): oss << "USA";break;
		case(CONVENTION::BONDBASIS): oss << "BONDBASIS"; break;
		case(CONVENTION::EUROPEAN): oss << "EUROPEAN"; break;
		case(CONVENTION::EUROBONDBASIS): oss << "EUROBONDBASIS"; break;
		case(CONVENTION::GERMAN): oss << "GERMAN"; break;
		case(CONVENTION::ITALIAN): oss << "ITALIAN"; break;		
		default:throw "Unknown convention type";
		}

		return oss.str();
	}

	double Thirty360::dayCount(const QDate& d1, const QDate& d2) const
	{
		return this->daycount_functor(d1, d2);
	}

	double Thirty360::yearfraction(const QDate& d1, const QDate& d2) const
	{
		return dayCount(d1, d2) / 360.0;
	}

	double Thirty360::daysperyear() const
	{
		return 360;
	}

	SimpleDayCounter::SimpleDayCounter()
	{
		fallback.reset(new Actual365fixed());
	}

	std::string SimpleDayCounter::name()
	{
		return "SimpleDayCounter";
	}

	double SimpleDayCounter::dayCount(const QDate& d1, const QDate& d2) const
	{
		return fallback->dayCount(d1, d2);
	}

	double SimpleDayCounter::yearfraction(const QDate& d1, const QDate& d2) const
	{
		int dm1 = d1.DayOfTheMonth();
		int dm2 = d2.DayOfTheMonth();

		if (dm1 == dm2 ||
			// e.g., Aug 30 -> Feb 28 ?
			(dm1 > dm2 && d2.IsMonthEnd()) ||
			// e.g., Feb 28 -> Aug 30 ?
			(dm1 < dm2 && d1.IsMonthEnd())) {
			return (d2.Year() - d1.Year()) +
				(int(d2.Month()) - int(d1.Month())) / 12.0;
		}
		else {
			return fallback->yearfraction(d1, d2);
		}
	}

	double SimpleDayCounter::daysperyear() const
	{
		return 360;
	}
}
