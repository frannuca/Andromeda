#pragma once

#include "libdef.h"
#include <functional>
#include "QDate.h"
#include "Calendar.h"

	namespace qtime {

		class  EXPORT_SYMBOL GermanCalendar:public Calendar {
		
		public:
			GermanCalendar();
			bool isBankHoliday(const QDate&) const override;			
		};
	}
