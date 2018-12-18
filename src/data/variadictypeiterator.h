#pragma once

#include <utility>
#include "Series.h"
#include <algorithm>
namespace data
{
	using namespace std;

	template<typename C, typename K, typename ... V>
	class VariadicTypeIterator: protected FrameValueTypes<C, K, V>... {
	protected:
		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		void ColumnsIndex2(map<string, vector<C>>& m)
		{
			this->FrameValueTypes<C, K, T1>::ColumnKeys(m);
			ColumnsIndex2<T2, T3, T4, T5>(m);
		}

		template<typename T2, typename T3, typename T4, typename T5>
		void ColumnsIndex2(map<string, vector<C>>& m)
		{
			this->FrameValueTypes<C, K, T2>::ColumnKeys(m);
			ColumnsIndex2<T3, T4, T5>(m);
		}

		template<typename T3, typename T4, typename T5>
		void ColumnsIndex2(map<string, vector<C>>& m)
		{
			this->FrameValueTypes<C, K, T3>::ColumnKeys(m);
			ColumnsIndex2<T4, T5>(m);
		}

		template<typename T4, typename T5>
		void ColumnsIndex2(map<string, vector<C>>& m)
		{
			this->FrameValueTypes<C, K, T4>::ColumnKeys(m);
			ColumnsIndex2<T5>(m);
		}

		template<typename T5>
		void ColumnsIndex2(map<string, vector<C>>& m)
		{
			this->FrameValueTypes<C, K, T5>::ColumnKeys(m);
		}

		template<typename T1, typename T2, typename T3, typename T4>
		void RowIndex2(map<string, vector<K>> &m)
		{
			this->FrameValueTypes<C, K, T1>::RowKeys(m);
			RowIndex2<T2, T3, T4>(m);
		}

		template<typename T2, typename T3, typename T4>
		void RowIndex2(map<string, vector<K>> &m)
		{
			this->FrameValueTypes<C, K, T2>::RowKeys(m);
			RowIndex2<T3, T4>(m);
		}

		template<typename T3, typename T4>
		void RowIndex2(map<string, vector<K>> &m)
		{
			this->FrameValueTypes<C, K, T3>::RowKeys(m);
			RowIndex2<T4>(m);
		}

		template<typename T4>
		void RowIndex2(map<string, vector<K>> &m)
		{
			this->FrameValueTypes<C, K, T4>::RowKeys(m);
		}
	};
}