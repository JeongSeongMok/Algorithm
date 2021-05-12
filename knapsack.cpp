#include <iostream>
#include <fstream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <queue>
#include <ctime>
#include <cmath>
#define MAX_SIZE 12000
#define TIME_LIMIT 15*60
#define TOTAL_TIME_LIMIT 70*60
using namespace std;
int num_of_item[] = { 100,1000,2000,4000,6000,8000,10000,12000};
ofstream fout("output.txt");

class item {
public:
	int w, v;
	bool operator <(item &data){
		return (double(this->v)/double(this->w)) > (double(data.v)/double(data.w));
	}
	double v_per_w(){
		return (double(v)/double(w));
	}
};

class bb_node{
public:
	int index;
	double bound;
	int tot_weight;	
	int curr_weight;
	double curr_value;
	void set(int i, double b, int tw, int cw, int cv){
		index=i, bound=b, tot_weight=tw, curr_weight=cw, curr_value=cv;
	}
};

struct compare {
	bool operator()(const bb_node &data1, const bb_node &data2){
		return data1.bound < data2.bound;
	}
};

void dp(int size, item data[MAX_SIZE+1], int W) {   //dynamic programming
	int **dp_memo;
	dp_memo = new int*[MAX_SIZE*40+1];
	for(int i=0; i<=MAX_SIZE*40;i++){
		dp_memo[i]=new int[MAX_SIZE+1];
	}
	int result=0;
	clock_t start = clock(), end;
	double duration;
	for(int j=0;j<=size;j++){
		for(int i=0;i<=W;i++){
			if(i==0 || j==0) {dp_memo[i][j]=0; continue;}
			if(data[j].w<=i) dp_memo[i][j] = max(dp_memo[i-data[j].w][j-1]+data[j].v,dp_memo[i][j-1]);
			else dp_memo[i][j]= dp_memo[i][j-1];
			result=max(result,dp_memo[i][j]);

			
		}
	}
end = clock();
			duration = double(end-start)/CLOCKS_PER_SEC;
			if(duration>TIME_LIMIT) {
				fout<<"\t\tTIMEOUT\t\t";
				return;  
			}
	fout.precision(9);
	fout<<"\t"<<result;
	fout.precision(6);
	fout<<"\t"<<round(duration*1000)/1000<<"ms";
	for(int i=0;i<MAX_SIZE+1;i++){
		delete [] dp_memo[i];
	}
	delete [] dp_memo;
return;
}

void greedy(int size, item input[MAX_SIZE+1], int W){  // greedy algorithm
	item data[MAX_SIZE+1];
	for(int i=1; i<=size;i++){
		data[i] = input[i];
	}
	
	clock_t start = clock(), end;
	double duration;
	sort(data+1,data+size+1);
	int curr_weight=0;
	double result=0;
	for(int i=1;i<=size;i++){
		
		if(curr_weight+data[i].w<=W){
			curr_weight+=data[i].w;
			result += double(data[i].v);
		}
		else {
			result+=(W-curr_weight)*data[i].v_per_w();
			break;
		}
	}
end = clock();
		duration = double(end-start)/CLOCKS_PER_SEC;
		if(duration>TIME_LIMIT) {
			fout<<"\t\tTIMEOUT"<<"\t";
			//printf("greedy: TIMEOUT\n");
			return;  
		}	
	fout.precision(9);
	fout<<"\t"<<result;
	fout.precision(6);
	fout<<"\t"<<round(duration*1000)/1000<<"ms\t";
return;
}

void branch_bound(int size, item input[MAX_SIZE+1], int W){  // branch and bound
	priority_queue <bb_node, vector<bb_node>, compare> q;
	item data[MAX_SIZE+1];
	for(int i=1; i<=size;i++){
		data[i] = input[i];
	}
	clock_t start = clock(), end;
	double duration;
	sort(data+1, data+size+1);

	bb_node temp;
	temp.tot_weight=0;
	temp.bound=0;
	for(int i=1;i<=size;i++){
		if(temp.tot_weight+data[i].w<=W){
			temp.tot_weight+=data[i].w;
			temp.bound+=data[i].v;
		}
		else{
			temp.bound+=(W-temp.tot_weight)*data[i].v_per_w();
			break;
		}
	}
	temp.set(0,temp.bound,temp.tot_weight, 0 ,0);

	
	q.push(temp);
	double max_benefit=0;
	while(!q.empty()){
		bb_node curr_node = q.top();
		if(curr_node.bound<=max_benefit || curr_node.index>=size) {
			q.pop();
			continue;
		}
		double new_bound = curr_node.curr_value;
		int new_tot = curr_node.curr_weight;
		if(curr_node.curr_weight+data[curr_node.index+1].w<=W){
			max_benefit = max(max_benefit,curr_node.curr_value+data[curr_node.index+1].v );
			for(int i=curr_node.index+1;i<=size;i++){
				if(new_tot+data[i].w<=W){
					new_tot+=data[i].w;
					new_bound +=data[i].v;
				}
				else{
					new_bound +=(W-new_tot)*data[i].v_per_w();
					break;
				}
			}
			if(max_benefit<new_bound){
				temp.set(curr_node.index+1, new_bound, new_tot,curr_node.curr_weight+data[curr_node.index+1].w,curr_node.curr_value+data[curr_node.index+1].v );
				q.push(temp);
			}
		}
		new_bound = curr_node.curr_value;
		new_tot = curr_node.curr_weight;
		for(int i=curr_node.index+2;i<=size;i++){
			if(new_tot+data[i].w<=W){
				new_tot+=data[i].w;
				new_bound +=data[i].v;
			}
			else{
				new_bound +=(W-new_tot)*data[i].v_per_w();
				break;
			}
		}
		if(max_benefit<new_bound){
			temp.set(curr_node.index+1, new_bound, new_tot, curr_node.curr_weight, curr_node.curr_value);
			q.push(temp);
		}
		q.pop();
		
	}
end = clock();
		duration = double(end-start)/CLOCKS_PER_SEC;
		if(duration>TIME_LIMIT) {
			fout<<"\t\t\tTIMEOUT\t";
			return;  
		}
	fout.precision(9);
	fout<<"\t\t"<<max_benefit;
	fout.precision(6);
	fout<<"\t"<<round(duration*1000)/1000<<"ms\t";
return ;
}

int main() {	
	clock_t total_start = clock(), total_end;
	fout<<"Num Of Items\t\t\tGreedy\t\t\t\t\tD.P.\t\t\t\t\tB.&B.\n";
	
	item data[MAX_SIZE+1];
	for (int cases = 0;cases < 8;cases++) {
		total_end = clock();
		double total_duration = double(total_end-total_start)/CLOCKS_PER_SEC;
		if(total_duration>TOTAL_TIME_LIMIT) {
			return 0;  
		}
		srand(time(NULL));
		for (int i = 1;i <= num_of_item[cases];i++) {
			data[i].v = rand() % 300 + 1;
			data[i].w = rand() % 100 + 1;
		}
		int W = num_of_item[cases] * 40;

		fout<<"\t"<<num_of_item[cases]<<"\t\t";
		if(cases==0) fout<<"\t";
		greedy(num_of_item[cases], data, W);
		if(cases==0) fout<<"\t";
		dp(num_of_item[cases], data,W);
		if(cases==0) fout<<"\t";
		branch_bound(num_of_item[cases], data, W);
		fout<<"\n";
	}
	fout.close();
	return 0;
}

