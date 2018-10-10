#include "frequency.h"

qtime::Tenor<qtime::SDAY> qtime::toTenor(const Frequency& f)
{
	switch(f)
	{
	case NoFrequency: 
	case Once: return 0_days;
	case Annual: return 1_years;
	case Semiannual: return 6_months;	
	case Quarterly: return 3_months;
	case Bimonthly: return 2_months;
	case Monthly: return 1_months;		
	case Biweekly: return 2_weeks;
	case Weekly: return 1_weeks;;
	case EveryFourthMonth: return 4_months;
	case Daily: return 2_days;;
	case EveryFourthWeek: return 4_weeks;
	case OtherFrequency:	
	default: 
		throw "invalid frequency";
	}
}
