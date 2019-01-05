// 头文件
#include "pch.h"
#include "iostream"
#include "cstdio"
#include "string"
#include "vector"
#include "graphics.h"
#include "conio.h"
#include "windows.h"
#include "stdlib.h"
#include "string.h"
#include "algorithm"
#include "queue"
#include "stack"
using namespace std;
const int maxn = 1005;
const int inf = 0x3f3f3f3f;

// 结构体
// 账户管理 
struct account
	{
		// 用户名
		string User_name;
		// 密码 
		string Password;
		// 账号权限 
		// 1 管理员
		// 2 用户
		int privilege;
	};
// 地点信息
struct pos_inf
	{
		// 地点名字(name)和简介(pre_int)
		string name, pre_int;
		// 地点坐标
		int x, y;
		// 有效性
		bool useble;
		pos_inf() { useble = false; }
	};
// 路径信息
struct pathes
	{
		// 长度，中转次数
		int length, step;
		// 路径
		int path[maxn];
	};
// 边信息
struct edges
	{
		//所连位置编号
		int id;
		//边长度
		int length;
	};
// bfs中间结构体
struct node
{
	int id;
	int step;
};

// 变量
// 依次为账号，位置，边，路径总数
int tot_account = 0, tot_pos, tot_edge, tot_path;
// bfs和dijk的路径长度
int bfs_length, dijk_length;
// 账号
vector<account> acc;
// 位置
vector<pos_inf> pos;
// 所有路径
vector<pathes> all_path;
// 无向图G
vector<edges> G[maxn];
// 保存路径
int path[maxn];
// 保存bfs路径
int path_bfs[maxn][maxn];
// 保存最短路径
int shortest_path[maxn];
// 保存最短路径
int best_path[maxn];
// 标记
int vis[maxn];
// 邻接矩阵
int mapp[maxn][maxn];
// 迪杰斯特拉中间变量
int pre[maxn][maxn];
// 迪杰斯特拉距离
int dis[maxn];

// 函数
// 载入账户
void load_account();
// 载入位置信息
void load_pos();
// 载入路径信息
void load_edge();
// 保存账户
void save_account();
// 保存位置信息
void save_pos();
// 保存路径信息
void save_edge();
// 隐藏密码
string input_password();
// 根据用户名查找账户
int account_find_name(string name);
// 密码比对
bool account_pascmp(string name, string Password);
// 登录
void sign_in();
// 注册
void sign_up();
// 二级菜单
void menu(int id);
// 账户管理菜单
void manage_account_menu(int id);
// 地图管理菜单
void manage_map_menu(int id);
// 输入位置信息
void write_pos();
// 输入路径信息
void write_edge();
// 打印位置（测试用）
void print_pos();
// 打印边（测试用）
void print_edge();
// 打印地图（测试用）
void draw_map();
// 根据地点名找到对应编号
int find_id(string s);
// string转wstring
wstring s2ws(const string& s);
// 打印所有路径
void show_all_path();
// 深度优先搜索
void dfs(int u, int v, int step, int length);
// 广度优先搜索
void bfs(int u, int v);
// 迪杰斯特拉
void Dijkstra(int s, int n);
// 迪杰斯特拉路径输出
void Dijkstra_path(int s, int v);
// 找所有路径
void find_all_path();
// 找最佳路径
void find_best_path_dfs();
// 找最佳路径(dijkstra)
void find_best_path_dijkstra();
// 找最短路径(dfs)
void find_shortest_path_dfs();
// 找最短路径(bfs)
void find_shortest_path_bfs();
// 按路径长度排序
bool cmp_best(pathes a1, pathes a2);
// 按中转次数排序
bool cmp_shortest(pathes a1, pathes a2);
// 查询账户
void view_account();
// 删除账户
void delete_account();
// 增加地点
void add_pos();
// 删除地点
void delete_pos();
// 增加路径
void add_edge();
// 删除路径
void delete_edge();


// 主函数
int main()
{
	load_edge();
	load_pos();
	load_account();
	freopen("CON", "r", stdin);
	string ch;
	// 主界面
	while (1)
	{
		//清屏
		system("cls");

		cout << endl;
		cout << "*******欢迎使用校园导航系统*******" << endl;
		cout << "*                                *" << endl;
		cout << "*            1. 注册             *" << endl;
		cout << "*            2. 登录             *" << endl;
		cout << "*            0. 退出             *" << endl;
		cout << "*                                *" << endl;
		cout << "**********************************" << endl << endl;
		cout <<	"           请输入操作:";

		cin >> ch;

		if (ch == "1")
			sign_up();
		else if (ch == "2")
			sign_in();
		else if (ch == "0")
			break;
		else
		{
			MessageBox(0, _T("请输入正确的数字"), _T("提示"), 0);
			/*
			printf("请输入正确的数字\n");
			printf("按任意键返回!");
			_getch();
			*/
		}
	}
	save_account();
	save_edge();
	save_pos();
	return 0;
}

// string转wstring
wstring s2ws(const string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	return r.c_str();
}

// 载入账户
void load_account()
{
	freopen("account.txt", "r", stdin);
	cin >> tot_account;
	if (tot_account == 0)
	{
		struct account temp;
		temp.User_name = "admin";
		temp.Password = "111111";
		temp.privilege = 1;
		tot_account = 1;
		acc.push_back(temp);
		fclose(stdin);
		return;
	}
	struct account temp;
	for (int i = 0; i < tot_account; i++)
	{
		cin >> temp.User_name;
		cin >> temp.Password;
		cin >> temp.privilege;
		acc.push_back(temp);
	}
	fclose(stdin);
}

// 保存账户
void save_account()
{
	freopen("account.txt", "wt", stdout);
	cout << tot_account << endl;
	struct account temp;
	for (int i = 0; i < tot_account; i++)
	{
		temp = acc[i];
		cout << temp.User_name << endl;
		cout << temp.Password << endl;
		cout << temp.privilege << endl;
	}
	fclose(stdout);
}

// 隐藏密码
string input_password()
{
	string Password = "";
	while (1)
	{
		char ch = _getch();
		if (ch == '\r' && Password.length() != 0)
			break;
		else if (ch == '\b')
		{
			if (Password.length() <= 0)
			{
				printf("\a");
				continue;
			}
			printf("\b \b");
			Password = Password.substr(0, Password.length() - 1);
			continue;
		}
		else if (ch!='\r')
		{
			if (Password.length() >= 16) continue;
			Password += ch;
			printf("*");
			//printf("%c", ch);
		}
	}
	return Password;
}

// 按路径长度排序
bool cmp_best(pathes a1, pathes a2)
{
	return a1.length < a2.length;
}

// 按中转次数排序
bool cmp_shortest(pathes a1, pathes a2)
{
	return a1.step < a2.step;
}

// 根据用户名查找账户
int account_find_name(string name)
{
	for (int i = 0; i < tot_account; i++)
	{
		if (acc[i].User_name == name)
			return i;
	}
	return -1;
}

// 载入边信息
void load_edge()
{
	freopen("edge.txt", "r", stdin);
	int u, v, len;
	cin >> tot_edge;
	for (int i = 0; i < tot_edge; i++)
	{
		cin >> u >> v >> len;
		edges tv, tu;
		tv.id = v; tv.length = len;
		tu.id = u; tu.length = len;
		G[u].push_back(tv);
		G[v].push_back(tu);
	}
	fclose(stdin);
}

// 输入位置信息
void write_pos()
{
	freopen("position.txt", "w", stdout);
	cin >> tot_pos;
	cout << tot_pos << endl;
	for (int i = 0; i < tot_pos; i++)
	{
		pos_inf temp; temp.useble = true;
		cin >> temp.name >> temp.pre_int >> temp.x >> temp.y;
		pos.push_back(temp);
		cout << temp.name << " "
			<< temp.pre_int << " "
			<< temp.x << " "
			<< temp.y << endl;
	}
	fclose(stdout);
}

// 输入路径信息
void write_edge()
{
	freopen("edge.txt", "w", stdout);
	int u, v, len;
	cin >> tot_edge;
	cout << tot_edge << endl;
	for (int i = 0; i < tot_edge; i++)
	{
		cin >> u >> v >> len;
		cout << u << " " << v << " " << len << endl;
		edges tv, tu;
		tv.id = v; tv.length = len;
		tu.id = u; tu.length = len;
		G[u].push_back(tv);
		G[v].push_back(tu);
	}
	fclose(stdout);
}

// 载入位置信息
void load_pos()
{
	//打开文件
	freopen("position.txt", "r", stdin);
	cin >> tot_pos;
	for (int i = 0; i < tot_pos; i++)
	{
		pos_inf temp;
		//读入各个位置的信息
		cin >> temp.name >> temp.pre_int >> temp.x >> temp.y;
		pos.push_back(temp);
	}
	fclose(stdin);
}

// 保存边信息
void save_edge()
{
	freopen("edge.txt", "w", stdout);
	cout << tot_edge << endl;
	for (int i = 0; i < tot_edge; i++)
	{
		for (int j = 0; j < G[i].size(); j++)
		{
			int u = i, v = G[i][j].id, len = G[i][j].length;
			if (u < v)
				cout << u << " " << v << " " << len << endl;
		}
	}
	fclose(stdout);
}

// 保存位置信息
void save_pos()
{
	//打开文件
	freopen("position.txt", "w", stdout);
	cout << tot_pos;
	for (int i = 0; i < tot_pos; i++)
	{
		pos_inf temp = pos[i];
		cout << temp.name << " " << temp.pre_int << " " << temp.x << " " << temp.y << endl;
	}
	fclose(stdout);
}

// 根据地点名找到对应编号
int find_id(string s)
{
	for (int i = 0; i < tot_pos; i++)
	{
		if (pos[i].name == s)
			return i;
	}
	return -1;
}

// 密码比对
bool account_pascmp(string name, string Password)
{
	for (int i = 0; i < tot_account; i++)
	{
		if (acc[i].User_name == name)
		{
			if (acc[i].Password == Password)
				return true;
			else
				return false;
		}
	}
	return false;
}
	
// 登录
void sign_in()
{
	string User_name, Password;
	while (1)
	{
		system("CLS");
		printf("提示:输入0返回\n");
		printf("用户名:\n");
		cin >> User_name; getchar();

		if (User_name == "0") return;

		if (account_find_name(User_name) == -1)
		{
			printf("用户名不存在!\n");
			printf("按任意键返回!\n");
			fflush(stdin); _getch();
			continue;
		}

		break;
	}

	while (1)
	{
		system("CLS");
		printf("提示:输入0返回\n");

		struct account now_account;
		int id = account_find_name(User_name);
		now_account = acc[id];

		printf("密码:\n");
		fflush(stdin);
		Password = input_password();

		if (Password == "0") return;

		if (account_pascmp(User_name, Password))
		{
			printf("\n按任意键继续!\n");
			fflush(stdin); getchar();

			menu(id);
			return;
		}
		else
		{
			printf("\n密码错误!\n");
			printf("输入任意键返回!\n");
			fflush(stdin); getchar();
			system("CLS");
			continue;
		}
	}
}

// 注册
void sign_up()
{
	string User_name, Password, Password_for_sure;
	while (1)
	{
		system("CLS"); fflush(stdin);
		printf("提示:输入0返回\n");
		printf("用户名:\n");
		cin >> User_name; getchar();

		if (account_find_name(User_name)!=-1)
		{
			printf("用户名已存在!\n");
			printf("按任意键返回!\n");
			fflush(stdin); getchar();
			continue;
		}

		if (User_name == "0") return;

		cout << "将" << User_name << "设为你的用户名(y/n):";
		fflush(stdin);
		char ch = getchar();
		if (ch == 'y' || ch == 'Y') break;
	}
	while (1)
	{
		system("CLS"); fflush(stdin);
		printf("提示:输入0返回\n");
		printf("密码的长度需要在[6,16]之间\n");
		printf("密码:\n");
		//scanf("%s",Password);
		fflush(stdin);
		Password = input_password();
		//printf("%s\n",Password);		

		if (Password == "0") return;

		if (Password.length() < 6)
		{
			printf("\n太短了!\n");
			printf("按任意键返回!\n");
			fflush(stdin); _getch();
			continue;
		}

		while (1)
		{
			system("CLS"); fflush(stdin);
			printf("提示:输入0返回\n");
			printf("请确认你的密码:\n");
			fflush(stdin);
			Password_for_sure = input_password();
			//scanf("%s",Password_for_sure);

			if (Password_for_sure == "0") return;

			if (Password == Password_for_sure)
			{
				struct account temp;

				temp.User_name = User_name;
				temp.Password = Password;
				temp.privilege = 2;
				acc.push_back(temp); tot_account++;
				//printf("tot_account=%d\n",tot_account); _getch();

				//save_account();

				printf("\n成功!\n");
				printf("按任意键继续!\n");
				fflush(stdin); _getch();
				return;
			}
			else
			{
				printf("\n两次密码不一致!\n");
				printf("按任意键返回!\n");
				fflush(stdin); _getch();
				continue;
			}
		}
	}
}

// 二级菜单
void menu(int id)
{
	string ch;
	// 主界面
	while (1)
	{
		//freopen("CON", "w", stdout);
		//清屏 
		system("cls");
		cout << "欢迎你," << acc[id].User_name << endl << endl;
		cout << "*******************************" << endl;
		cout << "*        1. 查看地图          *" << endl;
		cout << "*        2. 查询所有路径      *" << endl;
		cout << "*        3. 查询最短路径      *" << endl;
		cout << "*        4. 查询最佳路径      *" << endl;
		cout << "*        5. 账户管理          *" << endl;
		cout << "*        6. 地图管理          *" << endl;
		cout << "*        0. 退出              *" << endl;
		cout << "*******************************" << endl << endl;
		cout << "请输入操作:" << endl;

		cin >> ch; getchar();

		if (ch == "1")
			draw_map();
		else if (ch == "2")
			find_all_path();
		else if (ch == "3")
			find_shortest_path_bfs();
		else if (ch == "4")
			find_best_path_dijkstra();
		else if (ch == "5")
			manage_account_menu(id);
		else if (ch == "6")
			manage_map_menu(id);
		else if (ch == "0")
			return;
		else
		{
			MessageBox(0, _T("请输入正确的数字"), _T("提示"), 0);
			/*
			printf("请输入正确的数字\n");
			printf("按任意键返回!");
			_getch();*/
		}
	}
}

// 账户管理菜单
void manage_account_menu(int id)
{
	if (acc[id].privilege != 1)
	{
		printf("没有足够的权限!!!\n");
		printf("按任意键返回!");
		_getch();
		return;
	}
	string ch;
	// 主界面
	while (1)
	{
		system("cls");
		cout << "欢迎你," << acc[id].User_name << endl << endl;
		cout << "*******************************" << endl;
		cout << "*        1. 查询账户          *" << endl;
		cout << "*        2. 删除账户          *" << endl;
		cout << "*        0. 退出              *" << endl;
		cout << "*******************************" << endl << endl;
		cout << "请输入操作:" << endl;

		cin >> ch; getchar();

		if (ch == "1")
			view_account();
		else if (ch == "2")
			delete_account();
		else if (ch == "0")
			return;
		else
		{
			MessageBox(0, _T("请输入正确的数字"), _T("提示"), 0);
			/*
			printf("请输入正确的数字\n");
			printf("按任意键返回!");
			_getch();*/
		}
	}
}

// 地图管理菜单
void manage_map_menu(int id)
{
	if (acc[id].privilege != 1)
	{
		printf("没有足够的权限!!!\n");
		printf("按任意键返回!");
		_getch();
		return;
	}
	string ch;
	// 主界面
	while (1)
	{
		system("cls");
		cout << "欢迎你," << acc[id].User_name << endl << endl;
		cout << "*******************************" << endl;
		cout << "*        1. 增加地点          *" << endl;
		cout << "*        2. 删除地点          *" << endl;
		cout << "*        3. 增加路径          *" << endl;
		cout << "*        4. 删除路径          *" << endl;
		cout << "*        0. 退出              *" << endl;
		cout << "*******************************" << endl << endl;
		cout << "请输入操作:" << endl;

		cin >> ch; getchar();

		if (ch == "1")
			add_pos();
		else if (ch == "2")
			delete_pos();
		else if (ch == "3")
			add_edge();
		else if (ch == "4")
			delete_edge();
		else if (ch == "0")
			return;
		else
		{
			MessageBox(0, _T("请输入正确的数字"), _T("提示"), 0);
			/*
			printf("请输入正确的数字\n");
			printf("按任意键返回!");
			_getch();
			*/
		}
	}
}

// 查询账户
void view_account()
{
	system("cls");
	cout << "编号\t用户名" << endl;
	for (int i = 0; i < tot_account; i++)
	{
		printf("%d\t", i);
		cout << acc[i].User_name << endl;
	}
	_getch();
}

// 删除账户
void delete_account()
{
	int num;
	string str;
	system("cls");
	cout << "编号\t用户名" << endl;
	for (int i = 0; i < tot_account; i++)
	{
		printf("%d\t", i);
		cout << acc[i].User_name << endl;
	}
	while (cout << "请输入要删除的用户编号(输入-1退出):" << endl)
	{
		cin >> num;
		if (num == -1)
			return;
		if (num < 0 || num >= tot_account)
		{
			cout << "请输入正确的编号!" << endl;
			cout << "按任意键返回" << endl;
			_getch();
			system("cls");
		}
		else if (num == 0)
		{
			cout << "无法删除管理员" << endl;
			cout << "按任意键返回" << endl;
			_getch();
			return;
		}
		else break;
	}
	str = acc[num].User_name;
	vector<account>::iterator it;
	for (it = acc.begin(); it != acc.end(); it++)
	{
		if ((*it).User_name == str)
		{
			acc.erase(it);
			break;
		}
	}
	tot_account--;
	cout << "删除成功" << endl;
	cout << "按任意键返回" << endl;
	_getch();
	return;
}

// 找所有路径
void find_all_path()
{ 
	int u, v;
	system("cls");
	tot_path = 0; all_path.clear();
	memset(vis, 0, sizeof(vis));
	memset(path, 0, sizeof(path));
	for (int i = 0; i < tot_pos; i++)
		cout << i << " " << pos[i].name << endl;
	cout << "请输入起点和终点:" << endl;
	cin >> u >> v; getchar();
	path[0] = u; vis[u] = 1;
	dfs(u, v, 0, 0);
	sort(all_path.begin(), all_path.end(),cmp_best);
	system("cls");
	show_all_path();
}

// 打印所有路径
void show_all_path()
{
	if (tot_path == 0)
	{
		cout << "未查询到路径！" << endl;
		cout << "按任意键返回" << endl;
		_getch();
		return;
	}
	cout << "总共有" << tot_path << "条路径" << endl;
	for (int cnt = 0; cnt < tot_path; cnt++)
	{
		cout << "路径" << cnt+1 << "(距离" << all_path[cnt].length << "m):";
		for (int i = 0; i <= all_path[cnt].step; i++)
		{
			cout << pos[all_path[cnt].path[i]].name;
			if (i != all_path[cnt].step)
				cout << "-->";
		}
		getchar();
	}
}

// 找最佳路径(dfs)
void find_best_path_dfs()
{
	int u, v;
	system("cls");
	tot_path = 0; all_path.clear();
	memset(vis, 0, sizeof(vis));
	memset(path, 0, sizeof(path));
	for (int i = 0; i < tot_pos; i++)
		cout << i << " " << pos[i].name << endl;
	cout << "请输入起点和终点:" << endl;
	cin >> u >> v; getchar();
	path[0] = u; vis[u] = 1;
	dfs(u, v, 0, 0);
	if (all_path.size() == 0)
	{
		cout << "未查询到路径！" << endl;
		cout << "按任意键返回" << endl;
		_getch();
		return;
	}
	sort(all_path.begin(), all_path.end(), cmp_best);
	cout <<"距离:" << all_path[0].length << "m" << endl;
	for (int i = 0; i <= all_path[0].step; i++)
	{
		cout << pos[all_path[0].path[i]].name;
		if (i != all_path[0].step)
			cout << "-->";
	}
	getchar();
}

// 找最佳路径(dijkstra)
void find_best_path_dijkstra()
{
	int u, v;
	system("cls"); dijk_length = 0;
	memset(vis, 0, sizeof(vis));
	for (int i = 0; i < tot_pos; i++)
		cout << i << " " << pos[i].name << endl;
	cout << "请输入起点和终点:" << endl;
	cin >> u >> v; getchar();
	if (u<0 || u>=tot_pos || v<0 || v>=tot_pos)
	{
		cout << "未查询到路径！" << endl;
		cout << "按任意键返回" << endl;
		_getch();
		return;
	}
	Dijkstra(u, tot_pos);
	Dijkstra_path(u, v);
	if (dijk_length == 0)
	{
		cout << "未查询到路径！" << endl;
		cout << "按任意键返回" << endl;
		_getch();
		return;
	}
	cout << "距离:" << dis[v] << "m" << endl;
	for (int i = 0; i < dijk_length; i++)
	{
		cout << pos[best_path[i]].name;
		if (i != dijk_length-1)
			cout << "-->";
	}
	getchar();
}

// 找最短路径(dfs)
void find_shortest_path_dfs()
{
	int u, v;
	system("cls");
	tot_path = 0; all_path.clear();
	memset(vis, 0, sizeof(vis));
	memset(path, 0, sizeof(path));
	for (int i = 0; i < tot_pos; i++)
		cout << i << " " << pos[i].name << endl;
	cout << "请输入起点和终点:" << endl;
	cin >> u >> v; getchar();
	path[0] = u; vis[u] = 1;
	dfs(u, v, 0, 0);
	if (all_path.size() == 0)
	{
		cout << "未查询到路径！" << endl;
		cout << "按任意键返回" << endl;
		_getch();
		return;
	}
	sort(all_path.begin(), all_path.end(), cmp_shortest);
	cout << "最少次数:" << all_path[0].step << "次" << endl;
	for (int i = 0; i <= all_path[0].step; i++)
	{
		cout << pos[all_path[0].path[i]].name;
		if (i != all_path[0].step)
			cout << "-->";
	}
	getchar();
}

// 找最短路径(bfs)
void find_shortest_path_bfs()
{
	int u, v;
	system("cls");
	tot_path = 0; all_path.clear();
	memset(vis, 0, sizeof(vis));
	memset(path, 0, sizeof(path));
	for (int i = 0; i < tot_pos; i++)
		cout << i << " " << pos[i].name << endl;
	cout << "请输入起点和终点:" << endl;
	cin >> u >> v; getchar();
	bfs(u, v);
	if (bfs_length == 0)
	{
		cout << "未查询到路径！" << endl;
		cout << "按任意键返回" << endl;
		_getch();
		return;
	}
	cout << "最少次数:" << bfs_length-1 << "次" << endl;
	for (int i = 0; i < bfs_length; i++)
	{
		cout << pos[shortest_path[i]].name;
		if (i != bfs_length-1)
			cout << "-->";
	}
	getchar();
}

// 深度优先搜索
void dfs(int u, int v, int step, int length)
{
	path[step] = u;
	if (u == v)
	{
		pathes temp;
		temp.length = length;
		tot_path++;
		temp.step = step;
		for (int i = 0; i <= step; i++)
			temp.path[i] = path[i];
		all_path.push_back(temp);
		/*
		cout << "路径" << ++tot_path << "(距离" << length << "):";
		for (int i = 0; i <= step; i++)
		{
			cout << pos[path[i]].name;
			if (i != step)
				cout << "-->";
		}
		cout << endl;
		getchar();*/
		return;
	}
	for (int i = 0; i < G[u].size(); i++)
	{
		if (!vis[G[u][i].id])
		{
			vis[G[u][i].id] = 1;
			dfs(G[u][i].id, v, step + 1, length + G[u][i].length);
			vis[G[u][i].id] = 0;
		}
	}
}

// 广度优先搜索
void bfs(int u,int v)
{
	bfs_length = 0;
	memset(vis, 0, sizeof(vis));
	queue<node> Q;
	node temp; temp.id = u; temp.step = 0;
	Q.push(temp); vis[u] = 1;
	while (!Q.empty())
	{
		node now = Q.front(); Q.pop();
		if (now.id == v)
		{
			stack<int> ans; ans.push(now.id);
			int temp = now.id;
			for (int i = now.step; i >= 1; i--)
			{
				ans.push(path_bfs[i][temp]);
				temp = path_bfs[i][temp];
			}
			bfs_length = 0;
			while (!ans.empty())
			{
				shortest_path[bfs_length++] = ans.top();
				ans.pop();
			}
			//_getch();
			return;
		}
		for (int i = 0; i < G[now.id].size(); i++)
		{
			if (!vis[G[now.id][i].id])
			{
				vis[G[now.id][i].id] = 1;
				node new_node;
				new_node.id = G[now.id][i].id;
				new_node.step = now.step + 1;
				Q.push(new_node);
				path_bfs[now.step+1][G[now.id][i].id] = now.id;
			}
		}
	}
}

// 迪杰斯特拉
void Dijkstra(int s, int n)
{
	//memset(dis, 0, sizeof(dis));
	memset(mapp, inf, sizeof(mapp));
	for (int i = 0; i < tot_pos; i++)
	{
		for (int j = 0; j < G[i].size(); j++)
		{
			mapp[i][G[i][j].id] = mapp[G[i][j].id][i] = G[i][j].length;
			pre[i][G[i][j].id] = i;
			pre[G[i][j].id][i] = G[i][j].id;
		}
	}
	memset(vis, 0, sizeof(vis));
	for (int i = 0; i < n; i++)
		dis[i] = mapp[s][i];
	dis[s] = 0; vis[s] = 1;

	for (int i = 0; i < n; i++)
	{
		int k=n, tmp = inf;
		for (int j = 0; j < n; j++)
		{
			if (!vis[j] && dis[j] < tmp)
			{
				tmp = dis[j];
				k = j;
			}
		}
		vis[k] = 1;

		for (int j = 0; j < n; j++)
			if (!vis[j] && dis[j] > dis[k] + mapp[k][j])
			{
				dis[j] = dis[k] + mapp[k][j];
				pre[s][j] = pre[k][j];
			}
	}
}

// 迪杰斯特拉路径输出
void Dijkstra_path(int s,int v)
{
	if (s == v)
	{
		best_path[dijk_length++] = v;
		return;
	}
	Dijkstra_path(s,pre[s][v]);
	best_path[dijk_length++]=v;
}

// 打印位置（测试用）
void print_pos()
{
	cout << tot_pos << endl;
	for (int i = 0; i < tot_pos; i++)
	{
		pos_inf temp = pos[i];
		cout << temp.name << " "
			<< temp.pre_int << " "
			<< temp.x << " "
			<< temp.y << endl;
	}
}

// 打印边（测试用）
void print_edge()
{
	cout << tot_edge << endl;
	for (int i = 0; i < maxn; i++)
	{
		int len = G[i].size();
		for (int j = 0; j < len; j++)
		{
			cout << i << " " << (G[i][j]).id << " " << (G[i][j]).length << endl;
		}
	}
}	

// 打印地图
void draw_map()
{
	initgraph(700, 700);
	setbkcolor(BLACK);
	cleardevice();
	for (int i = 0; i < tot_pos; i++)
	{
		int x1, y1;
		x1 = (int)(0.7*pos[i].x);
		y1 = (int)(0.7*(1000 - pos[i].y)) + 30;

		for (int j = 0; j < G[i].size(); j++)
		{
			int x2, y2;
			x2 = (int)(0.7*pos[G[i][j].id].x);
			y2 = (int)(0.7*(1000 - pos[G[i][j].id].y)) + 30;
			line(x1, y1, x2, y2);
		}
		setfillcolor(WHITE);
		fillcircle(x1, y1, 4);
		string s = pos[i].name;
		wstring stemp = s2ws(s);
		LPCWSTR result = stemp.c_str();
		outtextxy(x1, y1 + 10, result);
	}
	_getch();
	closegraph();
}

// 增加地点
void add_pos()
{
	system("cls");
	pos_inf temp;
	while (cout << "请输入要增加的位置名称(输入-1退出):" << endl)
	{
		cin >> temp.name;
		if (temp.name == "-1")
			return;
		else if (find_id(temp.name) != -1)
		{
			cout << "位置已存在!!!" << endl;
			cout << "按任意键返回" << endl;
			_getch();
			system("cls");
		}
		else
			break;
	}
	cout << "请输入位置描述(输入-1退出):" << endl;
	cin >> temp.pre_int;
	if (temp.pre_int == "-1") return;
	cout << "请输入位置的横纵坐标(0<x<1000,0<y<1000)(输入-1退出):" << endl;
	cin >> temp.x;
	if (temp.x == -1) return;
	cin >> temp.y;
	pos.push_back(temp); tot_pos++;
	cout << "添加成功" << endl;
	cout << "按任意键继续" << endl;
	_getch();
}

// 删除地点
void delete_pos()
{
	int num;
	system("cls");	
	printf("编号\t地名\n");
	for (int i = 0; i < tot_pos; i++)
	{
		printf("%d\t", i);
		cout << pos[i].name << endl;
	}
	while (cout << "请输入要删除的地点编号(输入-1退出):" << endl)
	{
		cin >> num;
		if (num == -1)
			return;
		if (num < 0 || num >= tot_pos)
		{
			cout << "请输入正确的编号!" << endl;
			cout << "按任意键返回" << endl;
			_getch();
		}
		else break;
	}
	memset(mapp, -1, sizeof(mapp));
	for (int i = 0; i < tot_pos; i++)
	{
		for (int j = 0; j < G[i].size(); j++)
			mapp[i][G[i][j].id] = mapp[G[i][j].id][i] = G[i][j].length;
		G[i].clear();
	}
	tot_edge = 0;
	for (int i = 0; i < tot_pos; i++)
	{
		for (int j = i+1 ; j < tot_pos; j++)
		{
			if (mapp[i][j] == -1) continue;
			if (i == num || j == num) continue;
			edges tu,tv;
			tot_edge++;
			tu.id = i; tu.length = mapp[i][j];
			tv.id = j; tv.length = mapp[i][j];
			if (tu.id > num) tu.id--;
			if (tv.id > num) tv.id--;
			G[i].push_back(tv);
			G[j].push_back(tu);
		}
	}
	pos.erase(pos.begin() + num);
	tot_pos--;
	cout << "删除成功" << endl;
	cout << "按任意键返回" << endl;
	_getch();
	return;
}

// 增加路径
void add_edge()
{
	system("cls");
	printf("编号\t地名\n");
	for (int i = 0; i < tot_pos; i++)
	{
		printf("%d\t", i);
		cout << pos[i].name << endl;
	}
	int u,v,len;
	while (cout << "请输入要增加的边及路程(输入-1退出):" << endl)
	{
		cin >> u;
		if (u == -1)
			return;
		else
		{
			cin >> v >> len;
			if (u==v || 0 > u || u > tot_pos || 0 > v || v > tot_pos || len<=0)
			{
				cout << "请输入正确的位置及路程!!!" << endl;
				cout << "按任意键返回" << endl;
				_getch();
			}
			else
			{
				edges tv, tu;
				tv.id = v; tv.length = len;
				tu.id = u; tu.length = len;
				G[u].push_back(tv);
				G[v].push_back(tu);
				tot_edge++;
				cout << "添加成功" << endl;
				cout << "按任意键继续" << endl;
				_getch();
				return;
			}
			system("cls");
			printf("编号\t地名\n");
			for (int i = 0; i < tot_pos; i++)
			{
				printf("%d\t", i);
				cout << pos[i].name << endl;
			}
		}
	}
}

// 删除路径
void delete_edge()
{
	system("cls");
	printf("起点\t终点\t路程\n");
	for (int i = 0; i < tot_pos; i++)
		for (int j = 0; j < G[i].size(); j++)
			if (i<G[i][j].id)
				printf("%d\t%d\t%d\n", i, G[i][j].id, G[i][j].length);
	int u, v;
	while (cout << "请输入要删除的边的起点和终点(输入-1退出):" << endl)
	{
		cin >> u;
		if (u == -1)
			return;
		else
		{
			cin >> v;
			if (u == v || 0 > u || u > tot_pos || 0 > v || v > tot_pos)
			{
				cout << "请输入正确的位置及路程!!!" << endl;
				cout << "按任意键返回" << endl;
				_getch();
			}
			else
			{
				vector<edges>::iterator it,it1;
				for (it = G[u].begin(); it != G[u].end(); it++)
				{
					if ((*it).id == v)
					{
						G[u].erase(it);
						tot_edge--;
						for (it1 = G[v].begin(); it1 != G[v].end(); it1++)
						{
							if ((*it1).id == u)
							{
								G[v].erase(it1);
								break;
							}
						}
						cout << "删除成功" << endl;
						cout << "按任意键继续" << endl;
						_getch();
						return;
					}
					else if (it == G[u].end())
					{
						cout << "请输入正确的位置及路程!!!" << endl;
						cout << "按任意键返回" << endl;
						_getch();
						continue;
					}
				}
			}
			system("cls");
			printf("编号\t地名\n");
			for (int i = 0; i < tot_pos; i++)
			{
				printf("%d\t", i);
				cout << pos[i].name << endl;
			}
		}
	}
}