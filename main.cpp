#include "header.h"
#include "human.cpp"

int main(){
    init(2,2);

    vector<int> needs = {1,1};
    vector<int> prio = {0,1};
    vector<float> price = {2.0, 2.0};
    vector<int> inv = {0,0};
    int produtividade = 2;
    int dinheiro = 10;
    int hunger = 10;
    float sway = 0.5;
    float prod_total = 0;
    float prod_rate = 0.2;


    state Brazil(100, 0.1);

    // for (int i = 0; i < N; i++){
 
    //     humano.pb(
    //         human(i, dinheiro, i%2, sway, produtividade, needs, prio, price, inv)
    //     );
    // }
    humano.pb(
        human(last_id, dinheiro, hunger, 0, sway, produtividade, needs, prio, {3,1}, inv)
    );
    humano.pb(
        human(last_id, dinheiro, hunger, 1, sway, produtividade, needs, prio, {1,3}, inv)
    );


    int r = 1;
    int R = 20;


    while (R >= r){
        cout << "Start of loop " << r << "\n";

        prod_total = att_prod(prod_total, prod_rate);
        all("produce");
        all("checkin");
        all("seek_trade");
        all("checkout");
        log();

        if (vivos == 0){
            cout << "End of Loop at " << r << "\n";
            break;

        }

        r++;
    }
    for (int i = 0; i < G; i++){
        // cout << humano[0].price[i] << "    ";
        // cout << humano[1].price[i] << "\n";

    }

    return 0;
}