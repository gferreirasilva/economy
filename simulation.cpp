#include "header.h"
#include "entities_and_ai.cpp"

int main()
{

    vector<int> generic_needs = {1, 1};
    vector<int> generic_priority = {0, 1};
    vector<float> generic_prices = {2.0, 2.0};
    vector<int> generic_inventory = {0, 0};
    int generic_productivity = 10;
    int generic_money = 50;
    int generic_hunger = 0;
    float generic_sensitivity = 0.5;
    float generic_total_prod = 0;

    float rate_prod = 0.05;
    float max_prod = 1.2;

    int state_money = 10;
    float tax = 0.05;
    state gov(state_money, tax);

    int setup_people, setup_goods;
    setup_people = 2;
    setup_goods = 2;
    init(setup_people, setup_goods, rate_prod, max_prod);

    for (int i = 0; i < setup_people; i++)
    {
        humano.pb(
            human(last_id, generic_money, generic_hunger, 0, generic_sensitivity, generic_productivity, generic_needs, generic_priority, {3, 1}, generic_inventory));
    }
    for (int i = 0; i < setup_goods; i++)
    {
        products.pb(
            product(20, 1, 1, 1.0, 10, 20, 0.1, 30, {0, 0}));
    }
    int r = 1;
    int R = 10;

    while (R >= r)
    {
        cout << "Start of loop " << r << "\n";

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