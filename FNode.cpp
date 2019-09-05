#include "FNode.h"
using namespace std;
MNode::MNode(const string p_name, const vector <string> &str_list, const vector<function<void()>  > &vfunc_list) :
Name(p_name),
v_str(str_list),
v_func(vfunc_list)
{};

void MNode::Do(const int i) const 
{
	if ((i < 1) || (i > v_func.size()) || (i > v_str.size()))
	{
		cout << "Bad input, ty again" << endl;
		return;
	}
	else
		v_func[i - 1]();
};

void MNode::show() const 
{
	cout << "\n" << Name << " menu:" << endl;
	for (int i = 0; i < v_str.size(); ++i) 
	{
		cout << " " << i + 1 << " - " << v_str[i];
		cout << endl;
	}
	cout << " 0 - return;" << endl;
};

void MNode::Run() const 
{
	string in;
	show();
	cin >> in;
	while (atoi(in.c_str())) 
	{
		Do(atoi(in.c_str()));
		show();
		cin >> in;
	}
};