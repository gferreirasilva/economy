#include "header.h"
#include "entities_and_ai.cpp"

int main()
{

    vector<int> generic_needs = {1, 1};
    vector<int> generic_priority = {0, 1};
    vector<float> generic_prices = {2.0, 2.0};
    vector<int> generic_inventory = {3, 3};
    vector<int> generic_security = {3, 3};
    int generic_money = 50;
    int generic_hunger = 2;
    float generic_sensitivity = 0.5;
    float generic_productivity_rate = 1;

    //    int state_money = 10;
    //    float tax = 0.05;
    //   state gov(state_money, tax);

    int setup_people, setup_goods;
    setup_people = 2;
    setup_goods = 2;
    init(setup_people, setup_goods);

    for (int i = 0; i < setup_goods; i++)
    {
        products.pb(
            product(goods_zero, 20, 20, 0.1, 2.0));
    }

    for (int i = 0; i < setup_people; i++)
    {
        humankind.pb(
            human(
                generic_money,
                generic_hunger,
                i,
                generic_sensitivity,
                generic_productivity_rate,
                generic_needs,
                {1, 1},
                generic_priority,
                generic_inventory,
                generic_security));
    }

    int r = 1;
    int R = 4;

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
    //for (int i = 0; i < G; i++)
    //{
    // cout << humano[0].price[i] << "    ";
    // cout << humano[1].price[i] << "\n";
    //}

    return 0;
}