#pragma once
template <class __Iter, class __Func>
void ForEach(__Iter p_First, __Iter p_Last, __Func p_Op)
{
	for (; p_First != p_Last; ++p_First)
	{
		p_Op(*p_First);
	}
};