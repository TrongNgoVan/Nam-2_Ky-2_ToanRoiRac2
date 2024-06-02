#include <iostream>
#include<vector>
using namespace std;

#define nl "\n"

class state{

    // checks if the given indices are valid
    bool is_valid(int x, int y){
        return x >= 0 && x <= 2 && y >= 0 && y <= 2;
    }

    // swaps two integers
    void swap(int &a, int &b){
        int c = a;
        a = b;
        b = c;
    }

    // returns the manhattan distance between two points
    int manhattan_distance(pair<int, int> &a, pair<int, int> &b){
        return abs(a.first - b.first) + abs(a.second - b.second);
    }

    public:

    // the current state
    vector<vector<int>> table;

    state(){
        // default parameter
        for(int i = 0; i < 3; i++){
            vector<int> v(3);
            table.push_back(v);
        }
    }

    // returns the heuristic value for the current state
    // uses the manhattan distance between the position of each tile in the current state and the final state
    int heuristic(state &final){
        vector<pair<int, int>> pos_curr(9), pos_final(9);
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                pos_curr[table[i][j]] = {i, j};
                pos_final[final.table[i][j]] = {i, j};
            }
        }
        int cost = 0;
        for(int i = 0; i < 9; i++){
            cost += manhattan_distance(pos_curr[i], pos_final[i]);
        }
        return cost;
    }

    // returns a vector containing all the nieghbours of the current state
    vector<state> generate_neighbours(){
        vector<state> neighbours;
        int params[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1 ,0}};
        int x, y;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(table[i][j] == 0){
                    x = i;
                    y = j;   
                }
            }
        }
        for(int i = 0; i < 4; i++){
            int newx = x + params[i][0], newy = y + params[i][1];
            if(!is_valid(newx, newy))
                continue;
            state temp;
            temp.table = table;
            swap(temp.table[newx][newy], temp.table[x][y]);
            neighbours.push_back(temp);
        }
        return neighbours;
    }

    // prints the current state
    void display(){
        cout << nl;
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                cout << table[i][j] << " ";
            }
            cout << nl;
        }
    }

};

void hill_climb(state &initial, state &final){

    initial.display();
    int best = initial.heuristic(final);
    if(best == 0){
        cout << "\nThe final state has been reached.\n";
        return;
    }

    vector<state> neighbours = initial.generate_neighbours();
    state next;
    next.table = initial.table;


    for(auto &neighbour: neighbours){
        int curr = neighbour.heuristic(final);
        if(curr < best){
            best = curr;
            next = neighbour;
        }
    }

    if(next.table == initial.table){
        cout << "Stuck at a local minimum! Could not find any further path.\n";
        return;
    }
   
    hill_climb(next, final);

}

int main(){

    state initial;
    state final;
    
    // initial.table = {{1, 2, 3}, {4, 5, 6}, {0, 7, 8}};

    // final.table = {{1, 2, 3}, {4, 5, 6}, {7, 8, 0}};

    cout << "Enter the initial state(use 0 for empty tile):\n";

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++)
            cin >> initial.table[i][j];
    }

    cout << "Enter the final state(use 0 for empty tile):\n";

    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++)
            cin >> final.table[i][j];
    }

    cout << "\nThe path is:\n";

    hill_climb(initial, final);

}
