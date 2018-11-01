#include "buinessdayconvention.h"

qtime::BusinessDayConvention::BusinessDayConvention(const Calendar* calendar, TYPE t):p_calendar_(calendar),rule(t)
{
}

qtime::QDate qtime::BusinessDayConvention::adapt(const QDate& d) const
{	
	return adapt_internal(d, this->rule);
}

qtime::QDate qtime::BusinessDayConvention::adapt_internal(const QDate& d, TYPE t) const
{
	QDate x;
	switch (t)
	{
	case TYPE::NoAdjustment:
		return d;
	case TYPE::Previous:
		if (p_calendar_->isBusinessDay(d)) { return d; }
		else
		{
			x = d - 1;
			while (!p_calendar_->isBusinessDay(x))
			{
				x = x - 1;
			}
			return x;
		}
	case TYPE::Following:
		if (p_calendar_->isBusinessDay(d)) { return d; }
		else
		{
			x = d + 1;
			while (!p_calendar_->isBusinessDay(x))
			{
				x = x + 1;
			}
			return x;
		}
	case TYPE::ModifiedPrevious:
		if (p_calendar_->isBusinessDay(d)) { return d; }
		else
		{
			auto previous = adapt_internal(d, TYPE::Previous);
			if(previous.Month() != d.Month())
			{
				return adapt_internal(d, TYPE::Following);
			}
			else
			{
				return previous;
			}
		}
	case TYPE::ModifiedFollowing:
		if (p_calendar_->isBusinessDay(d)) { return d; }
		else
		{
			auto following = adapt_internal(d, TYPE::Following);
			if (following.Month() != d.Month())
			{
				return adapt_internal(d, TYPE::Previous);
			}
			else
			{
				return following;
			}
		}
	case TYPE::EndofMonthNoAdjustment:
		return d.MonthEnd();
	case TYPE::EndofMonthPrevious:
		x = d.MonthEnd();
		if(p_calendar_->isBusinessDay(x))
		{
			return x;
		}
		else
		{
			return adapt_internal(x, TYPE::Previous);
		}
	case TYPE::EndofMonthFollowing:
		x = d.MonthEnd();
		if (p_calendar_->isBusinessDay(x))
		{
			return x;
		}
		else
		{
			return adapt_internal(x, TYPE::Following);
		}
	default:
		throw "Invalid Business day Convention call";
	}
}

