#pragma once

#include "../inc/libdef.h"
#include <functional>
#include "QDate.h"


namespace qtime {

	class EXPORT_SYMBOL Calendar {
	public:
		virtual ~Calendar() = default;
		Calendar();
		virtual bool isBankHoliday(const QDate&)  const = 0;
		static bool isWeekend(const QDate&);
		bool isHoliday(const QDate&) const;
		bool isBusinessDay(const QDate&) const;
		QDate nextBusinessDay(const QDate& d) const;
		QDate prevBusinessDay(const QDate& d) const;
		static int EasterMonday(int year);
	};
}

