#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <cmath>
using namespace std;
// only for positive weight edge

class node{
public:
    string name;
    int num;
    int dist;
    node *next ;
};

class dnode{
public:
    int num;
    int dist;
};

struct compare {
	bool operator()(const dnode &data1, const dnode &data2){
        if(data1.dist<0) return true;
        if(data2.dist<0) return false;
		return data1.dist > data2.dist;
	}
};

node *v = new node[30];
ifstream f;
int get_first_line(string buffer){
    buffer+=" ";
    bool finishflag = false;
    string temp="";
    int index=0;
    for(int i=0;i<buffer.size();i++){
        if(buffer[i]<'A' || (buffer[i]>'Z' && buffer[i]<'a') || buffer[i]>'z'){
            if(i==buffer.size()-1 || finishflag){
               v[index].dist = 0;
               v[index].name = temp;
               v[index].next=0x0;
               temp="";
               index++;
               finishflag=false;
           }
        }
        else{
            temp+=buffer[i];
            finishflag=true;
        }
    }
    return index;
}

void make_ad_list(int v_num){
    string temp;
    int col=0, row=0;
    while(f>>temp){
        if(col!=0 && temp!="INF" && temp!="0"){
            node *temp_node = new node;
            temp_node->dist = atoi(&temp[0]);
            temp_node->name = v[col-1].name;
            temp_node->num = col-1;
            temp_node->next = 0x0;
            node *itr = &v[row];
            while(itr->next!=0x0) itr=itr->next;
            itr->next = temp_node;
       }
       if(col==v_num) {
           col=0;
           row++;
       }
       else col++;
    }
}

void print(int srcmat[30][30], int v_num, double duration, char* algo){
    printf("It took %f seconds to compute shortest path between cities with %s's algorithm as follows.\n\t\t",duration, algo);
    for(int i=0;i<v_num;i++) {
        if(v[i].name.length()<8) cout<<v[i].name<<"\t";
        else cout<<v[i].name;
    }
    cout<<"\n";
    for(int i=0;i<v_num;i++){
        if(v[i].name.length()<8) cout<<v[i].name<<"\t\t";
        else cout<<v[i].name<<"\t";
        for(int j=0; j<v_num;j++){
            cout<<srcmat[i][j]<<"\t";
        }
        cout<<"\n";
    }
    cout<<"\n";
}
void dijkstra(int v_num){
    string algoname="Dijkstra";
    bool visit[30];
    int result[30][30];
    dnode temp;
    clock_t start=clock(),end;
    for(int src=0; src<v_num; src++){
        memset(visit,false,sizeof(visit));
        vector <dnode> q; 
        temp.dist=0;
        temp.num=src;
        q.push_back(temp);
        for(int i=0;i<v_num;i++){
            if(i==src) continue;
            temp.dist=-1;
            temp.num=i;
            q.push_back(temp);
        }
        make_heap(q.begin(),q.end(),compare());
        while(!q.empty()){
            temp = q[0];
            result[src][temp.num] = temp.dist;
            q.erase(q.begin());
            visit[temp.num] = true;
            node *itr = v[temp.num].next;
            while(itr!=0x0){
                if(!visit[itr->num]){
                    int find_index=0;
                    while(find_index<q.size()){
                        if(q[find_index].num == itr->num) break;
                        find_index++;
                    }
                    if(q[find_index].dist==-1) q[find_index].dist = itr->dist+temp.dist;
                    else q[find_index].dist = min(q[find_index].dist,itr->dist+temp.dist);
                }
                make_heap(q.begin(),q.end(),compare());
                itr=itr->next;
            }
        }
    }
    end=clock();
    double duration = double(end-start)/CLOCKS_PER_SEC;    
    print(result,v_num, duration,&algoname[0]);
}



void floyd(int v_num){
    string algoname = "Floyd";
    int matrix[30][30];
    clock_t start = clock(),end;
    for(int i=0;i<v_num;i++){
        for(int j=0; j<v_num;j++){
            if(i==j) matrix[i][j]=0;
            else matrix[i][j]=-1;
        }
        node *itr = v[i].next;
        while(itr!=0x0){
            matrix[i][itr->num] = itr->dist;
            itr=itr->next;
        }
    }

    for(int src=0;src<v_num;src++){
        for(int i=0;i<v_num;i++){
            for(int j=0;j<v_num;j++){
                if(i==j||i==src||j==src||matrix[src][j]==-1 || matrix[i][src]==-1) continue;
                if(matrix[i][j]==-1) matrix[i][j] = matrix[src][j]+matrix[i][src];
                else matrix[i][j] = min(matrix[i][j], matrix[src][j]+matrix[i][src]);
            }
        }
    }
    end=clock();
    double duration = double(end-start)/CLOCKS_PER_SEC;
    print(matrix, v_num,duration,&algoname[0]);
}

int main(){
    f.open("hw6_data.txt");
    string temp;
    getline(f,temp);
    int v_num = get_first_line(temp);
    make_ad_list(v_num);
    
    dijkstra(v_num);
    floyd(v_num);

    f.close();
    return 0;
}