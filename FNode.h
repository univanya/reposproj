#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <String>
using namespace std;
class cls_Application;
class MNode 
{

	friend class cls_Application;
	string Name = "Empty";
	vector<string> v_str;
	vector<function<void()>> v_func;

	MNode(const string p_name = "Empty", const vector <string> &str_list = {}, const vector<function<void()>  > &vfunc_list = {});
	void Do(const int i) const;
	void show() const;
public:
	void Run() const;
};
