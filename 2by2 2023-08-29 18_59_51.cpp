/*This is a long term project to optimally solve a rubiks cube using a BFS
First need to figure out how I am gonna represent the cube
One way is an array of 8 pairs (a, b) where a is 1 to 8 and b is 1 to 3 based on the orientation
Might be a little complicated to handle?
Can a computer hold an array of that size?
size is like 3^8 * 8!
8! = 40320 so like 40320*6561 so 264,539,520 = 2.6e8 maybe a bit much - nah it managed fairly well
Chal ok back to the same question how do we represent?
Do we just move forward with this one?
How would a move R look in this representation
Firstly we need to define the things ig
Top view:
1 2 
4 3

(And Under those respectively)
5 6
8 7

so on move right pos[7] = pos[6], pos[6]=pos[2], pos[2]= pos[3]. Fair enough but what happens to the orientations
Urgh defining orinetations is gonna be messy
so firstly define normal position of rubiks cube having white up yellow at bottom and red in front
(so orange at back, blue on right, green on left ig)
1. Note that all sides either have white or they have yellow
so we could do based on that like if white/yellow is facing where it normally needs to face then its orientation 0
If its facing a position from where it needs to be turned anticlockwise wrt normal position to face correctly then orientation 1
and if clockwise then orientation 2
Ok how does move R affect orientations - we can just check how it works on a solved rubiks cube

fine so ok uhm this is confusing I guess I need a rubiks cube

so what's the plan after this

Can we convert these weird 16 element vectors into single numbers?
Should be doable but idk

oh shit we can't do an adj list
so we'll just hv to keep defining what are the neighbours of the current vertices using the R U F functions
but then we do require a visited thing at the very least - how the hell are you gonna do this

it'll look like visited[(0-8 range)][(0-8 range)][(0-8 range)][(0-8 range)][(0-8 range)][(0-8 range)][(0-8 range)][(0-8 range)][(0 - 2 val)]
Uhm there should be a better way
maybe a map?
can u even map a vector to an int - yes u can apparently Ig that shd work then
ok wow the map thing seems to be working so that's good. Now all we need to do is the dirty work of defining different moves on a 2*2*2 rubiks cube

Ok update I've written most of the code. I'm a bit scared cuz I don't think this is gonna work that well, the queue I have can get really large 
Might hit 6*5^n in n moves ig - eh shd work for a few moves atleast - ok I turned everything to bytes so maybe a little more space yay

ORIENTATION 0 - WHITE/ YELLOW FACING UP/DOWN
ORIENTATION 1 - CLOCKWISE TWIST NEEDED TO GET WHITE/YELLOW
ORIENTATION 2 - ANTICLOCKWISE TWIST NEEDED TO GET WHITE/YELLOW


for 3  
*/


#include <iostream>
#include <bits/stdc++.h>
using namespace std;
//#define int long long
#define bytes unsigned char
//typedef bitset<3> bytes;
const int N = 40320*6561;
vector<pair<bytes,bytes>> current_vertex(8);
vector<pair<bytes,bytes>> parent_vertex(8);


void R(){
    bytes temp = current_vertex[1].first;
    current_vertex[1].first = current_vertex[2].first;
    current_vertex[2].first = current_vertex[6].first;
    current_vertex[6].first = current_vertex[5].first;
    current_vertex[5].first = temp;

    temp = current_vertex[1].second;
    current_vertex[1].second= (current_vertex[2].second+2)%3;
    current_vertex[2].second= (current_vertex[6].second+1)%3;
    current_vertex[6].second= (current_vertex[5].second+2)%3;
    current_vertex[5].second= (temp+1)%3;
}

void F(){
    bytes temp = current_vertex[2].first;
    current_vertex[2].first = current_vertex[3].first;
    current_vertex[3].first = current_vertex[7].first;
    current_vertex[7].first = current_vertex[6].first;
    current_vertex[6].first = temp;

    temp = current_vertex[2].second;
    current_vertex[2].second= (current_vertex[3].second+2)%3;
    current_vertex[3].second= (current_vertex[7].second+1)%3;
    current_vertex[7].second= (current_vertex[6].second+2)%3;
    current_vertex[6].second= (temp+1)%3;
    
}

void U(){
    bytes temp = current_vertex[1].first;
    current_vertex[1].first = current_vertex[0].first;
    current_vertex[0].first = current_vertex[3].first;
    current_vertex[3].first = current_vertex[2].first;
    current_vertex[2].first = temp;
    temp = current_vertex[1].second;
    current_vertex[1].second = current_vertex[0].second;
    current_vertex[0].second = current_vertex[3].second;
    current_vertex[3].second = current_vertex[2].second;
    current_vertex[2].second = temp;
}

void Rdash(){
    bytes temp = current_vertex[1].first;
    current_vertex[1].first = current_vertex[5].first;
    current_vertex[5].first = current_vertex[6].first;
    current_vertex[6].first = current_vertex[2].first;
    current_vertex[2].first = temp;

    


    temp = current_vertex[1].second;
    current_vertex[1].second= (current_vertex[5].second+2)%3;
    current_vertex[5].second= (current_vertex[6].second+1)%3;
    current_vertex[6].second= (current_vertex[2].second+2)%3;
    current_vertex[2].second= (temp+1)%3;
    // I hope I don't need to debug this later - EDIT: Much sad I had to
}

void Fdash(){
    F();
    F();
    F();
}

void Udash(){
    bytes temp = current_vertex[1].first;
    current_vertex[1].first = current_vertex[2].first;
    current_vertex[2].first = current_vertex[3].first;
    current_vertex[3].first = current_vertex[0].first;
    current_vertex[0].first = temp;
    temp = current_vertex[1].second;
    current_vertex[1].second = current_vertex[2].second;
    current_vertex[2].second = current_vertex[3].second;
    current_vertex[3].second = current_vertex[0].second;
    current_vertex[0].second = temp;
}


void print(){
    for(int i =0; i<8; i++){
        cout <<(int) current_vertex[i].first<<","<<(int) current_vertex[i].second<<endl;
    }
    cout <<endl;
}
void R2(){
    R(); R();
}
void U2(){
    U(); U();
}
void F2(){
    F(); F();
}
bool check_if_done(){
    bool check = true;

    vector<pair<bytes,bytes>> final_vertex(8);
    final_vertex =  {make_pair(0,0),
    make_pair(1,0),
    make_pair(2,0),
    make_pair(3,0),
    make_pair(4,0),
    make_pair(5,0),
    make_pair(6,0),
    make_pair(7,0)};
    if (current_vertex == final_vertex) return true;
    final_vertex =  {make_pair(1,0),
    make_pair(2,0),
    make_pair(3,0),
    make_pair(0,0),
    make_pair(5,0),
    make_pair(6,0),
    make_pair(7,0),
    make_pair(4,0)};
    if (current_vertex == final_vertex) return true;
     final_vertex =  {make_pair(2,0),
    make_pair(3,0),
    make_pair(0,0),
    make_pair(1,0),
    make_pair(6,0),
    make_pair(7,0),
    make_pair(4,0),
    make_pair(5,0)};
    if (current_vertex == final_vertex) return true;
    final_vertex =  {make_pair(3,0),
    make_pair(0,0),
    make_pair(1,0),
    make_pair(2,0),
    make_pair(7,0),
    make_pair(4,0),
    make_pair(5,0),
    make_pair(6,0)};
    if (current_vertex == final_vertex) return true;
    final_vertex =  {make_pair(7,0),
    make_pair(6,0),
    make_pair(5,0),
    make_pair(4,0),
    make_pair(3,0),
    make_pair(2,0),
    make_pair(1,0),
    make_pair(0,0)};
    if (current_vertex == final_vertex) return true;
    final_vertex =  {make_pair(6,0),
    make_pair(5,0),
    make_pair(4,0),
    make_pair(7,0),
    make_pair(2,0),
    make_pair(1,0),
    make_pair(0,0),
    make_pair(3,0)};
    if (current_vertex == final_vertex) return true;
    final_vertex =  {make_pair(5,0),
    make_pair(4,0),
    make_pair(7,0),
    make_pair(6,0),
    make_pair(1,0),
    make_pair(0,0),
    make_pair(3,0),
    make_pair(2,0)};
    if (current_vertex == final_vertex) return true;
    final_vertex =  {make_pair(4,0),
    make_pair(7,0),
    make_pair(6,0),
    make_pair(5,0),
    make_pair(0,0),
    make_pair(3,0),
    make_pair(2,0),
    make_pair(1,0)};
    if (current_vertex == final_vertex) return true;
    
    return false;
}
void set_to(){
    
    for(int i =0; i<8; i++){
        parent_vertex[i].first=current_vertex[i].first;
        parent_vertex[i].second=current_vertex[i].second;
    }
}
void make_jumble(){
    Rdash();
    F();
    Udash();
    Rdash();
    F();
    Udash();
    Rdash();    
    Udash();
    F();
    Udash();
    R();
    Fdash();
    U();
    Rdash();
}
signed main(){
    map<vector<pair<bytes,bytes>>,pair<int,vector<pair<bytes,bytes>>>> distance;
    vector<pair<bytes,bytes>> final_vertex(8);
     final_vertex =  {make_pair(0,0),
    make_pair(1,0),
    make_pair(2,0),
    make_pair(3,0),
    make_pair(4,0),
    make_pair(5,0),
    make_pair(6,0),
    make_pair(7,0)};

    current_vertex = 
    {make_pair(0,0),
    make_pair(1,0),
    make_pair(2,0),
    make_pair(3,0),
    make_pair(4,0),
    make_pair(5,0),
    make_pair(6,0),
    make_pair(7,0)};

    make_jumble();
    //Jumbled
    queue <vector<pair<bytes,bytes>>> q;

    q.push(current_vertex);
    distance[current_vertex]=make_pair(1, current_vertex);
    while(!q.empty()){
        current_vertex = q.front();
        int d = distance[current_vertex].first;
        set_to();      
        q.pop();

        if(check_if_done()){
            break;
        }
        R();
        if(distance[current_vertex].first!=0) Rdash();
        else{
            q.push(current_vertex);
            distance[current_vertex]=make_pair(d+1,parent_vertex);
            Rdash();
        }
        Rdash();
        if(distance[current_vertex].first!=0) R();
        else {
            q.push(current_vertex);
            distance[current_vertex]=make_pair(d+1,parent_vertex);
            R();
        }
        
        U();
        if(distance[current_vertex].first!=0) Udash();
        else{
            q.push(current_vertex);
            distance[current_vertex]=make_pair(d+1,parent_vertex);
            Udash();
        }
        
        Udash();
        if(distance[current_vertex].first!=0) U();
        else{
            q.push(current_vertex);
            distance[current_vertex]=make_pair(d+1,parent_vertex);
            U();
        }
        
        
        F();
        if(distance[current_vertex].first!=0) Fdash();
        else{
            q.push(current_vertex);
            distance[current_vertex]=make_pair(d+1,parent_vertex);
            Fdash();
        }
        
        Fdash();
        if(distance[current_vertex].first!=0) F();
        else{
            q.push(current_vertex);
            distance[current_vertex]=make_pair(d+1,parent_vertex);
            F();
        }
        F2();
        if(distance[current_vertex].first!=0) F2();
        else{
            q.push(current_vertex);
            distance[current_vertex]=make_pair(d+1,parent_vertex);
            F2();
        }
        R2();
        if(distance[current_vertex].first!=0) R2();
        else{
            q.push(current_vertex);
            distance[current_vertex]=make_pair(d+1,parent_vertex);
            R2();
        }
        U2();
        if(distance[current_vertex].first!=0) U2();
        else{
            q.push(current_vertex);
            distance[current_vertex]=make_pair(d+1,parent_vertex);
            U2();
        }
        
    }
    current_vertex = final_vertex;
    vector <string> moves(distance[final_vertex].first-1);
    int i = distance[final_vertex].first-1;
    while(true){
        i--;
        if (distance[current_vertex].second == current_vertex) break;
        parent_vertex = current_vertex;
        R();
        if(current_vertex == distance[parent_vertex].second){
            moves[i] = "R'";
            continue;
        }
        else Rdash();
        Rdash();
        if(current_vertex == distance[parent_vertex].second){
            moves[i] = "R";
            continue;
        }
        else R();
        U();
        if(current_vertex == distance[parent_vertex].second){
            moves[i] = "U'";
            continue;
        }
        else Udash();
        Udash();
        if(current_vertex == distance[parent_vertex].second){
            moves[i] = "U";
            continue;
        }
        else U();

        Fdash();
        if(current_vertex == distance[parent_vertex].second){
            moves[i] = "F";
            continue;
        }
        else F();
        F();
        if(current_vertex == distance[parent_vertex].second){
            moves[i] = "F'";
            continue;
        }
        else Fdash();
        F2();
        if(current_vertex == distance[parent_vertex].second){
            moves[i] = "F2";
            continue;
        }
        else F2();
        R2();
        if(current_vertex == distance[parent_vertex].second){
            moves[i] = "R2";
            continue;
        }
        else R2();
        U2();
        if(current_vertex == distance[parent_vertex].second){
            moves[i] = "U2";
            continue;
        }
        else U2();

    }
    for(auto p: moves){
        cout <<p <<" ";
    }
    cout <<endl<<"Number of moves = "<<distance[final_vertex].first-1;
    return 0;
}