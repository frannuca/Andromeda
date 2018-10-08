#pragma once
#include "libdef.h"
#include "QDate.h"

namespace qtime
{
	inline namespace literals
	{
		
		struct EXPORT_SYMBOL SDAY {
			double n; 
			static  constexpr double numberofdays = 1.0; 			
		};

		struct EXPORT_SYMBOL SMONTH { double n;static constexpr double numberofdays = 30.0;};
		struct EXPORT_SYMBOL SWEEK  { double n;static constexpr double numberofdays = 7.0;};
		struct EXPORT_SYMBOL SYEAR  { double n;static constexpr double numberofdays = 360.0;};

		template<typename Q>
		struct EXPORT_SYMBOL Tenor: public Q
		{			
			Tenor(const Tenor<Q> & t) { this->Q::n = t.Q::n; }									
			explicit Tenor(long double x) { this->Q::n = double(x); }
			Tenor<Q> operator=(const double& x) {return Tenor<Q>{x}; }
			Tenor<Q> operator+=(const Tenor<Q>& rhs) { return Tenor<Q>{this->Q::n + rhs}; }			
			Tenor<Q> operator-=(const Tenor<Q>& rhs) { return Tenor<Q>{this->Q::n - rhs}; }			
			template<typename P>
			operator Tenor<P>() { return Tenor<P>(this->Q::n * Q::numberofdays / P::numberofdays); }
		};		


		template<typename Q>
		bool operator==(const Tenor<Q>& lhs, const Tenor<Q>& rhs) { return std::abs(lhs.n - rhs.n)<1e-12; }

		template<typename Q>
		Tenor<Q> operator+(const Tenor<Q>& lhs,const Tenor<Q>& rhs) { return Tenor<Q>(lhs.n + rhs.n); }

		template<typename Q>
		Tenor<Q> operator-(const Tenor<Q>& lhs, const Tenor<Q>& rhs) { return Tenor<Q>(lhs.n - rhs.n); }

		template<typename Q>
		Tenor<Q> operator*(const Tenor<Q>& lhs, const double& l) { return Tenor<Q>(lhs.n * l); }

		template<typename Q>
		Tenor<Q> operator*(const double& l, const Tenor<Q>& rhs) { return Tenor<Q>(l*rhs.n); }

		template<typename Q>
		Tenor<Q> operator/(const Tenor<Q>& lhs,const double & l) { return Tenor<Q>(lhs.n / l); }

		template<typename Q>
		Tenor<Q> operator/(const double & l, const Tenor<Q>& rhs) { return Tenor<Q>(l/ rhs.n); }

		template<typename Q>
		Tenor<Q> operator*=(const double & l, const Tenor<Q>& rhs) { return Tenor<Q>(l*rhs.n); }

		template<typename Q>
		Tenor<Q> operator*=(const Tenor<Q>& lhs, const double & l) { return Tenor<Q>(lhs.n*l); }

		template<typename Q>
		Tenor<Q> operator/=(const double & l, const Tenor<Q>& rhs) { return Tenor<Q>(l/rhs.n); }

		inline QDate operator+(const QDate& x, const Tenor<SDAY>& d) { return x + int(d.n); };
		inline QDate operator+(const Tenor<SDAY>& d, const QDate& x) { return x + d; };

		inline QDate operator-(const QDate& x, const Tenor<SDAY>& d) { return x - int(d.n); };
		inline QDate operator-(const Tenor<SDAY>& d, const QDate& x) { return x - d; };
	

		inline QDate operator+(const QDate& x, const Tenor<SMONTH>& d) {
			int m_0 = int(x.Month())-1 + int(d.n);
			int newmonth = (m_0 % 12) + 1;
			double dy = m_0 / 12.0;					
			return QDate(x.DayOfTheMonth(), newmonth, x.Year() + floor(dy));			
		}

		inline QDate operator+(const Tenor<SMONTH>& d, const QDate& x) { return x + d; };

		inline QDate operator-(const QDate& x, const Tenor<SMONTH>& d) { return x + Tenor<SMONTH>(-d.n); };
		inline QDate operator-(const Tenor<SMONTH>& d, const QDate& x) { return x - d; };


		inline QDate operator+(const QDate& x, const Tenor<SYEAR>& d) { return QDate(x.DayOfTheMonth(), int(x.Month()), x.Year() + d.n); };
		inline QDate operator+(const Tenor<SYEAR>& d, const QDate& x) { return x + d; };

		inline QDate operator-(const QDate& x, const Tenor<SYEAR>& d) { return x - Tenor<SYEAR>(-d.n); };
		inline QDate operator-(const Tenor<SYEAR>& d, const QDate& x) { return x - d; };


		inline double to_years(const Tenor<SDAY>& o, int daysperyear = 365)
		{
			return o.n / daysperyear;
		}
		inline double to_years(const Tenor<SMONTH>& o, int daysperyear = 365) { return o.n / 12.0; }
		inline double to_years(const Tenor<SYEAR>& o, int daysperyear = 365) { return o.n; }
		inline double to_years(const Tenor<SWEEK>& o, int daysperyear = 365) { return o.n*7/daysperyear; }
		

		inline Tenor<SDAY> operator""_days(long double n) { return Tenor<SDAY>(n); }
		inline Tenor<SMONTH> operator""_months(long double n) { return Tenor<SMONTH>(n); }
		inline Tenor<SWEEK> operator""_weeks(long double n) { return Tenor<SWEEK>(n); }
		inline Tenor<SYEAR> operator""_years(long double n) { return Tenor<SYEAR>(n); }
	}
	

}
