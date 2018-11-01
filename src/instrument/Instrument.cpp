#include "Instrument.h"

instrument::Instrument::Instrument(qtime::QDate maturity_):maturity(maturity_)
{
}

instrument::Instrument::~Instrument()
{
}

instrument::CashFlow::CashFlow(const qtime::QDate& dm,const qtime::Tenor<qtime::SMONTH>& tenor, double nominal)
	: instrument::Instrument(dm),tenor(tenor),	nominal(nominal)	
{
}


instrument::CashFlow::~CashFlow()
{
}

instrument::Instrument::INSTRUMENTCLASS instrument::CashFlow::getclass()
{
	return INSTRUMENTCLASS::CASHFLOW;
}

instrument::Swap::Swap(const qtime::QDate& dm, const double& c, const CashFlow& floating_leg, const CashFlow& fixed_leg)
	:instrument::Instrument(dm), C(c),
	floating_leg(&floating_leg),
	fixed_leg(&fixed_leg)
{
}

instrument::Swap::~Swap()
{
}

instrument::Instrument::INSTRUMENTCLASS instrument::Swap::getclass()
{
	return INSTRUMENTCLASS::SWAP;
}

instrument::xIbor::xIbor(const qtime::QDate& dm, const double& c, const qtime::Tenor<qtime::SMONTH>& tenor)
	: instrument::Instrument(dm), 
	C(c),
	tenor(tenor)
{
}

instrument::xIbor::~xIbor()
{
}

instrument::Instrument::INSTRUMENTCLASS instrument::xIbor::getclass()
{
	return INSTRUMENTCLASS::xIBOR;
}

