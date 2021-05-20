#include <iostream>
#include <fstream>
using namespace std;

class node{
public:
    char name;
    node *next ;
};

void print(node *d, int num_v){
    for(int i=0;i<num_v;i++){
        node *itr = &d[i];
        while(itr!=0x0){
            cout<<itr->name<<"-> ";
            itr=itr->next;
        }
        cout<<"null"<<"\n";
    }
}

int main(){
    char temp;
    int num_v = 0;
    ifstream f;
    f.open("hw5_.data.txt");
    
    string buffer;
    getline(f,buffer);
    for(int i=0;i<buffer.size();i++){
        if(buffer[i]>='A' && buffer[i]<='Z') { num_v++;}
    }
    
    node *v= new node[num_v];
    node *p= new node[num_v];
    for(int i=0;i<num_v;i++){
        v[i].name = i+65;
        p[i].name = i+65;
        v[i].next = 0x0;
        p[i].next = 0x0;
    }
    int k=0;
    while(k<num_v){
        getline(f,buffer);
        int index=0;
        node *itr = &v[k];
        for(int i=0;i<buffer.size();i++){
            if(buffer[i]=='0') index++;
            else if(buffer[i]=='1') {
                node *new_node = new node;
                new_node->name = char(65+index);
                new_node->next = 0x0;
                itr->next = new_node;
                itr=itr->next;

                node *new_p_node = new node;
                new_p_node->name = char(65+k);
                new_p_node->next=0x0;
                node *itr_p = &p[index];
                while(itr_p->next!=0x0){
                    itr_p=itr_p->next;
                }
                itr_p->next = new_p_node;

                index++;
            }
        }
        k++;
    }
    cout<<"---------INPUT GRAPH----------\n";
    print(v,num_v);
    cout<<"\n";
    cout<<"---Transpose of INPUT GRAPH---\n";
    print(p,num_v);
    free(v);
    f.close();
    return 0;
}