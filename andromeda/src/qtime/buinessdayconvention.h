#pragma once
#include "Calendar.h"

namespace qtime
{
	class BusinessDayConvention
	{
	public:
		enum class TYPE {
			NoAdjustment = 0, Previous, Following, ModifiedPrevious, ModifiedFollowing,
			EndofMonthNoAdjustment, EndofMonthPrevious, EndofMonthFollowing
		};
		BusinessDayConvention() = delete;
		BusinessDayConvention(const Calendar* calendar, TYPE t);
		QDate adapt(const QDate& d) const;

	private:
		const Calendar* p_calendar_;
		const TYPE rule;
	    QDate adapt_internal(const QDate& d, TYPE t) const;
	};
}
