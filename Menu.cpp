#include "Menu.h"

cls_Application::cls_Application() 
{
	Nodes = { MNode() };
};
void cls_Application::MainNode(const vector <string> &str_list, const vector<function<void()>  > &vfunc_list) 
{
	Nodes[0] = (MNode("Main", str_list, vfunc_list));
};
void cls_Application::AddNode(const string p_name, const vector <string> &str_list, const vector<function<void()>  > &vfunc_list) 
{
	Nodes.push_back(MNode(string(p_name), str_list, vfunc_list));
};
void cls_Application::Run()const
{
	Nodes[0].Run();
};
const MNode& cls_Application::find(string N) const
{
	for (int i = 0; i < Nodes.size(); ++i)
		if (Nodes[i].Name == N)
			return Nodes[i];
	cerr << "cls_Application::operator()(string N) - error" << endl << "not found " << N << ", return empty Node" << endl;
	return Nodes[0];
};
function<void()> cls_Application::operator()(string N) const 
{
	return toNode(N);
};
function<void()> cls_Application::toNode(string N) const
{
	return [&, N]
	{
		system("cls");
		(this->find(N)).Run();
	};
};

