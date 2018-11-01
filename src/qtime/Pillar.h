#pragma once
#include "libdef.h"
#include "QDate.h"
namespace qtime
{
	template<typename T>
	struct Pillar
	{	
		QDate date;
		T value;		
	};
}