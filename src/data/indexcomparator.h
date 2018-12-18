#pragma once
template<typename K>
struct IndexComparer
{
	boost::optional<std::function<bool(const K&, const K&)>> fcomparer;

	IndexComparer()
	{
		fcomparer = [](const K& a, const K& b) {return a < b; };
	}
};