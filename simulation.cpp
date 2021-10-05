#include "header.h"
#include "rules_and_ai.cpp"

int main()
{
    init(2, 2);

    vector<int> needs = {1, 1};
    vector<int> prio = {0, 1};
    vector<float> price = {2.0, 2.0};
    vector<int> inv = {0, 0};
    int produtividade = 10;
    int dinheiro = 50;
    int hunger = 0;
    float sway = 0.5;
    float prod_total = 0;
    float prod_rate = 0.05;

    int state_money;
    int tax;

    state new_gov(state_money, tax);
    gov = new_gov;

    // for (int i = 0; i < N; i++){

    //     humano.pb(
    //         human(i, dinheiro, i%2, sway, produtividade, needs, prio, price, inv)
    //     );
    // }
    humano.pb(
        human(last_id, dinheiro, hunger, 0, sway, produtividade, needs, prio, {3, 1}, inv));
    humano.pb(
        human(last_id, dinheiro, hunger, 1, sway, produtividade, needs, prio, {1, 3}, inv));

    int r = 1;
    int R = 10;

    while (R >= r)
    {
        cout << "Start of loop " << r << "\n";

        prod_total = update_productivity(prod_total, prod_rate);
        all("production");
        all("checkin");
        all("seek_trade");
        all("checkout");
        log();

        if (alive == 0)
        {
            cout << "End of Loop at " << r << "\n";
            break;
        }

        r++;
    }
    for (int i = 0; i < G; i++)
    {
        // cout << humano[0].price[i] << "    ";
        // cout << humano[1].price[i] << "\n";
    }

    return 0;
}