#pragma once
#include "FNode.h"

using namespace std;
class cls_Application
{
	vector<MNode> Nodes;
	const MNode& find(string N) const;
public:
	cls_Application();
	void MainNode(const vector <string> &str_list = {}, const vector<function<void()>  > &vfunc_list = {});
	void AddNode(const string p_name = "Empty", const vector <string> &str_list = {}, const vector<function<void()>  > &vfunc_list = {});
	void Run() const;
	function<void()> operator()(string N) const;
	function<void()> toNode(string N) const;
};