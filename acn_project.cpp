#include<bits/stdc++.h>
#define MAX 100
using namespace std;
int no_of_nodes,links,sinknode,data_trans_rate,linkcapacity,nodecount,all_cong_flag,no_alt_path_flag;
int edgegraph[MAX][MAX];
int level[MAX];
int visited[MAX];
int TableSize[MAX];
int parent[MAX];
int congestion_flag[MAX];
int congestednodes[MAX];
int alternatevisit[MAX];
clock_t strt,stp;
struct Node
{
	int value;
	struct Node *next;
};
struct nbr
{
	int neighbourNode;
	int level;
};
struct nbrtbl
{
	nbr neigbr[MAX];
};
typedef struct Node node;
typedef struct nbr neighbour;
typedef struct nbrtbl neighbourtable;
node *graph[MAX];
neighbourtable Table[MAX];
void initialization()
{
	for(int i = 0;i<MAX;i++)
	{
		level[i] = 0;
		visited[i] = 0;
		TableSize[i] = 0;
		congestion_flag[i]= 0;
		congestednodes[i]=0;
		alternatevisit[i]=0;
		parent[i]=0;
		graph[i] = (node*)malloc(sizeof(node));
		graph[i]->next = nullptr;
	}
	for(int  i = 0;i<MAX;i++)
	for(int  j = 0;j<MAX;j++)
	edgegraph[i][j] = 0;
}
void prepareneighbourTable()
{
	cout<<"--------------------------------------------------------------------------------------------------------------\n";
	cout<<"                                       Neighbouring Table Generated\n";
	cout<<"--------------------------------------------------------------------------------------------------------------\n";
	for(int i = 0;i<no_of_nodes;i++)
	{
		node * p = graph[i+1]->next;
		int neighbrcntr = 0;
		while(p != nullptr)
		{
			Table[i+1].neigbr[neighbrcntr].neighbourNode = p->value;
			Table[i+1].neigbr[neighbrcntr++].level = level[p->value];
			p = p->next;
		}
		TableSize[i+1] = neighbrcntr;
	}
}
void displayNeighbourTable()
{
	cout<<"--------------------------------------------------------------------------------------------------------------\n";
	cout<<"                                     DisplayNeighbourTableforeachnode\n";
	cout<<"--------------------------------------------------------------------------------------------------------------\n";
	cout<<"current Node\t\t\t";
	cout<<"Adjacent Node\t    level\n";
	cout<<"---------------------------------------------------------------------------------------------------------------\n";
	for(int i = 0;i< no_of_nodes;i++)
	{
		int n = TableSize[i+1];
		cout<<i+1<<"\t\t";
		for(int j = 0;j<n;j++)
		{
			if(j==0)
			cout<<"\t\t\t";
			if(j!=0)
			cout<<"\t\t\t\t\t";
			cout<<Table[i+1].neigbr[j].neighbourNode<<"\t\t";
			cout<<Table[i+1].neigbr[j].level<<"\t\t\n";
		}
		cout<<"\n\n\n";
	}
}
void readGraph()
{
	cout<<"Enter no of nodes in the simulation\n";
	cin>>no_of_nodes;
	cout<<"Enter no of links in the simulation\n";
	cin>>links;
	int vi,vj;
	for(int i = 0;i<links;i++)
	{
		cout<<"Enter the connected nodes (vi,vj)\n";
		cin>>vi>>vj;
		cout<<"Enter channel capacity\n";
		cin>>linkcapacity;
		node *nu = (node *)malloc(sizeof(node));
		nu->value = vj;
		//nu->linkcapacity = linkcapacity;
		nu->next =nullptr;
		node *nu1 = (node *)malloc(sizeof(node));
		nu1->value = vi;
		//nu1->linkcapacity = linkcapacity;
		nu1->next =nullptr;
		if(graph[vi]->next == nullptr)
		graph[vi]->next = nu;
		else
		{
			nu->next=graph[vi]->next;
			graph[vi]->next = nu;
		}
		if(graph[vj]->next ==nullptr)
		graph[vj]->next =nu1;
		else
		{
			nu1->next=graph[vj]->next;
			graph[vj]->next = nu1;			
		}
		edgegraph[vi][vj]=linkcapacity;
		edgegraph[vj][vi]=linkcapacity;
	}
	cout<<"--------------------------------------------------------------------------------------------------------------------\n";
	cout<<"					                             Topology created\n";
	cout<<"--------------------------------------------------------------------------------------------------------------------\n";
}
void displayShortestPath()
{
	int host;
	cout<<"----------------------------------------------------------------------------------------------------------------------------\n";
	cout<<"                                   Enter the host for shortest path selection\n";
	cout<<"----------------------------------------------------------------------------------------------------------------------------\n";
	cin>>host;
	cout<<host<<"-->";
	while(parent[host] != 0)
	{
		cout<<parent[host]<<"-->";
		host = parent[host];
	}
	cout<<"NULL\n";	
}
void congestionpath(int n)
{
	int host = n;
	cout<<"\n";
	cout<<host<<"-->";
	while(parent[host] != 0)
	{
		cout<<parent[host]<<"-->";
		host = parent[host];
	}
	cout<<"NULL\n";	
}
void sinkNodeInitialization()
{
	nodecount = 0;
	all_cong_flag = 0;
	no_alt_path_flag = 0;
	for(int i = 0;i<MAX;i++)
	{
		visited[i] = 0;
		congestion_flag[i] = 0;
		level[i] = 0;
		parent[i]=0;
		congestednodes[i] = 0;
		//nodecount = 0;
		
	}
}
void checkforCongestion()
{
	node *check = graph[sinknode]->next;
	int tempcount = 0;
	while(check)
	{
		//cout<<check->value<<" ";
		//cout<<edgegraph[sinknode][check->value]<<" ";
		//cout<<endl;
		if(edgegraph[sinknode][check->value] <= data_trans_rate)
		{
			congestion_flag[check->value] = 1;
			tempcount++;
		}
		check=check->next;
	}
	for(int i =1;i<=no_of_nodes;i++)
	{
		int count = 0;
		for(int j = 1;j<=no_of_nodes;j++)
		{
				if(edgegraph[i][j] <= data_trans_rate)
				{
						count++;
				}
		}
		if(count == no_of_nodes)
		congestion_flag[i] = 1;
	}
	//cout<<"\n";
	//cout<<tempcount<<" "<<nodecount<<"\n";
	if(tempcount == nodecount)
	{
		all_cong_flag = 1;
		cout<<"-------------------------------------------------------------------------------------------------------------------------\n";
		cout<<"                                 All Routes are Congested,Kindly wait/decrease transmission rate                                                        \n";
		cout<<"-------------------------------------------------------------------------------------------------------------------------\n";
		for(int i = 0;i<no_of_nodes;i++)
		{
			congestion_flag[i+1] = 1;
		}
	}
	congestion_flag[sinknode]=0;
}
void sinkadjacentcount()
{
	node *temp = graph[sinknode]->next;
	while(temp)
	{
		nodecount++;
		temp=temp->next;
	}
}
void printAlternatepath(int n)
{
	if(n == sinknode)
	{
		cout<<n<<"--->NULL"<<"\n";
		return;
	}
	for(int i = 1;i<=no_of_nodes;i++)
	{
		if(edgegraph[n][i] > data_trans_rate && alternatevisit[i] == 0)
		{
				cout<<n<<"--->";
				no_alt_path_flag = 1;
				alternatevisit[n]=1;
				printAlternatepath(i);
		}
	}
	if(no_alt_path_flag == 0)
	{
		cout<<"---------------------------------------------------------------------------------------------------------------------------\n";
		cout<<"					No Alternate path possible\n";
		cout<<"---------------------------------------------------------------------------------------------------------------------------\n";
	}
}
void sinkNodeDistance()
{
	strt = clock();
	queue<int>qu;
	qu.push(sinknode);
	sinkNodeInitialization();
	visited[sinknode] = 1;
	parent[sinknode]=0;
	sinkadjacentcount();
	checkforCongestion();
	while(!qu.empty())
	{
		int quval  = qu.front();
		//cout<<quval<<" ";
		qu.pop();
		node * temp = graph[quval]->next;
		//cout<<temp->value<<" ";
		while(temp)
		{
			if(visited[temp->value] == 0)
			{
				qu.push(temp->value);
				visited[temp->value] = 1;
				level[temp->value] =level[quval]+1;
				parent[temp->value] = quval;
			}
			temp=temp->next;
		}
	}
	if(all_cong_flag == 0)
	{
		cout<<"\n";
		cout<<"----------------------------------------------------------------------------------------------------------------------\n";
		cout<<"Node\t\t\t"<<"Distance from Sink\n";
		cout<<"----------------------------------------------------------------------------------------------------------------------\n";
		for(int i = 0;i<no_of_nodes;i++)
		{
			if(congestion_flag[i+1] == 0)
			cout<<i+1<<"\t\t\t"<<level[i+1]<<"\n";
			else
			{
				cout<<i+1<<"\t\t\t"<<"Link Level Congestion\n";
				congestednodes[i+1] = 1;
			}
		}
		for(int i = 0;i<no_of_nodes;i++)
		{
			if(congestednodes[i+1] == 1)
			{
				cout<<"-------------------------------------------------------------------------------------------------------------\n";
				cout<<"                                    Path Before Congestion\n";
				cout<<"-------------------------------------------------------------------------------------------------------------\n";
				congestionpath(i+1);
				cout<<"-------------------------------------------------------------------------------------------------------------\n";
				cout<<"                                    Alternate Path                                                           \n";
				cout<<"-------------------------------------------------------------------------------------------------------------\n";
				printAlternatepath(i+1);
			}
		}
	}
	stp =clock();
}
void displayGraph()
{
	for(int i = 0;i<no_of_nodes;i++)
	{
		cout<<i+1<<"-->";
		node *temp =graph[i+1]->next;
		while(temp)
		{
			cout<<temp->value;
			cout<<"-->";
			temp=temp->next;
		}
		cout<<"NULL\n";
	}		
}
void calculateEfficiency()
{
	double tim = double(stp - strt)/CLOCKS_PER_SEC;
	//cout<<tim<<"\n";
	for(int i = 1;i<=no_of_nodes;i++)
	{
		cout<<"Time to deliver packet from host "<<i<<" is "<<level[i]*tim<<"\n";
	}
}
int main()
{
	int operation;
	cout<<"1.Read Graph\n2.Display Graph\n3.hop-distance to sink\n4.Table Creation\n5.Table Display\n6.Minimal Distance Display\n7.Calculate Time\n ";
	while(1)
	{
		cout<<"Enter your option\n";
		cin>>operation;
		switch(operation)
		{
			case 1	: 	initialization();
						readGraph();
						break;
			case 2	: 	displayGraph();
						break;
			case 3	:	cout<<"Enter sinkNode\n";
						cin>>sinknode;
						cout<<"Enter Data transmission Rate\n";
						cin>>data_trans_rate;
						level[sinknode] = 0;
						sinkNodeDistance();
						break;
			case 4	:	prepareneighbourTable();
						break;
			case 5	:	displayNeighbourTable();
						break;
			case 6	:	displayShortestPath();
						break;
			case 7  :   calculateEfficiency();
						break;
			case 8	: 	exit(0);
						break;
		}	
	}
	return 0;
}