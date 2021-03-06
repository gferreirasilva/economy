#include "header.h"

typedef pair<int, int> pii;

int N = 0;
int G = 0;
int total_exchanged = 0;
int total_money = 0;
int total_produced = 0;
int last_id = 0;
int alive = 0;

vector<int> supply, demand;

void init(int n, int g)
{
    N = n;
    G = g;
    alive = N;

    for (int i = 0; i < G; i++)
    {
        supply.pb(0);
        demand.pb(0);
    }
}

void reset()
{
    for (int t = 0; t < G; t++)
    {
        supply[t] = 0;
        demand[t] = 0;
    }
    total_produced = 0;
    total_exchanged = 0;
    total_money = 0;

    return;
}

class human
{
public:
    vector<int> needs, priority, inventory, dis;
    vector<float> price;
    vector<float> max_price;

    int life = 1;
    int starvation = 0;

    int id, productivity, job;
    float money, sensitivity, income;

    human(int identification, float initial_money, int hungry_limit, int initial_job, float initial_sensitivity, int initial_productivity, vector<int> setup_needs, vector<int> setup_priority, vector<float> initial_prices, vector<int> initial_inventory)
    {
        id = identification;
        money = initial_money;
        starvation = hungry_limit;
        job = initial_job;
        sensitivity = initial_sensitivity;
        productivity = initial_productivity;

        for (int i = 0; i < G; i++)
        {
            needs.pb(setup_needs[i]);
            priority.pb(setup_priority[i]);
            price.pb(initial_prices[i]);
            max_price.pb(initial_prices[i]);
            inventory.pb(initial_inventory[i]);
            dis.pb(0);
        }
        last_id++;
    }

    void checkin();
    void checkout();
    void seek_trade();
    void production();
    void request_aid(int cash);
};

vector<human> humano;

class state
{
public:
    vector<int> credit, debt;
    int money;
    float fees;

    state(int dinheiro, float taxas)
    {
        money = dinheiro;
        fees = taxas;
    }

    void tax()
    {
        int taxation;
        for (int i = 0; i < N; i++)
        {
            taxation = humano[i].money * fees;
            money += taxation;
            humano[i].money -= taxation;
        }
        return;
    }
    void print_money(int cash)
    {
        //To Dev
        //Conditions to analyze: Will this money encourage productivityuction in valuable sectors of the economy?
        //For now all requests for generating money are being accepted.
        money += cash;
        return;
    }
    void buy()
    {
        //To Dev
        return;
    }
    int donate(human asker, int grant)
    {
        //To Dev
        //Conditions to analyze: How much money do we have? How much money does the human have? What job does the human have?
        //For now all requests to donate money are being accepted.
        if (money >= grant)
        {
            money -= grant;
            return grant;
        }
        if (money <= grant)
        {
            //To Dev
            //Accordingly to the print_money() function, every time the state doesn't have enough money, it will just print more
            print_money(grant);
            money -= grant;
            return grant;
        }
        return 0;

        return 0;
    }

    void lend()
    {
        //To Dev
        return;
    }
    void inventoryest()
    {
        //To Dev
        return;
    }
};

state gov(0, 0);

void human::checkin()
{
    income = 0;
    total_money += money;
    income += price[job] * productivity;

    for (int q = 0; q < G; q++)
    {
        dis[q] += inventory[q] - needs[q];

        if (dis[q] <= 0)
        {
            demand[q] -= dis[q];
        }
        if (dis[q] > 0)
        {
            supply[q] += dis[q];
        }
    }

    return;
};
void human::checkout()
{
    for (int q = 0; q < G; q++)
    {
        if (-dis[q] > starvation)
        {
            request_aid(max_price[q]);
            if (max_price[q] <= money)
            {
                seek_trade();
            }
        }

        if (-dis[q] > starvation)
        {
            life = 0;
            alive--;
            cout << id << " died\n";
            return;
        }
        inventory[q] -= needs[q];
    }

    return;
}

void trade(int id_buyer, int id_seller, int quantity, int product)
{
    human buyer = humano[id_buyer];
    human seller = humano[id_seller];
    int dif = seller.price[product] - buyer.price[product];

    if (seller.dis[product] <= 0)
    {
        //To Dev
        //Early interpretation of inflation and loss of consumption power
        //Maybe it happens when you don't productivityuce it too?
        if (seller.job == product)
        {
            seller.price[product] += dif * seller.sensitivity;
            humano[id_seller] = seller;
        }
        return;
    }
    quantity = min(quantity, -buyer.dis[product]);

    int price = seller.price[product];

    if (price > buyer.price[product])
    {

        if (-buyer.dis[product] >= buyer.starvation)
        {
            //Halfing the buyer sensitivity to compensate the fact that he is obligated to buy at that price.
            buyer.price[product] += dif * buyer.sensitivity * 0.5;
        }

        else
        {
            buyer.max_price[product] = seller.price[product];
            buyer.price[product] += dif * buyer.sensitivity;
            seller.price[product] -= dif * buyer.sensitivity;
            humano[id_buyer] = buyer;
            humano[id_seller] = seller;

            return;
        }
    }

    buyer.inventory[product] += quantity;
    seller.inventory[product] -= quantity;

    buyer.money -= price;
    seller.money += price;

    buyer.dis[product] += quantity;
    seller.dis[product] -= quantity;

    total_exchanged += quantity * price;

    humano[id_buyer] = buyer;
    humano[id_seller] = seller;
};

void human::production()
{
    //To Dev
    //Simples productivityuction, not capable of handling complex materials.
    inventory[job] += productivity;
    total_produced += price[job] * productivity;

    return;
}

void human::seek_trade()
{

    for (int item = 0; item < G; item++)
    {
        int it = priority[item];

        if (dis[it] > 0)
        {
            continue;
        }

        for (int k = 0; k < N; k++)
        {
            if (id == k || humano[k].life == 0)
            {
                continue;
            }
            trade(id, k, humano[k].dis[it], it);
        }
    }
}

void human::request_aid(int grant)
{
    int value = gov.donate(humano[id], grant);
    money += value;
    return;
}

float update_productivity(float productivity_total, float productivity_rate)
{
    //To Dev
    //Linear Progession of productivity and not capable of handling complex materials
    productivity_total += productivity_rate;
    if (productivity_total >= 1)
    {
        int atual = productivity_total;
        productivity_total = 0;

        for (int i = 0; i < N; i++)
        {
            humano[i].productivity += atual;
        }
        return 0;
    }
    return productivity_total;
}

void log()
{
    for (int i = 0; i < G; i++)
    {
        cout << "Product: " << i << " ";
        cout << "Demand: " << demand[i] << " ";
        cout << "Supply: " << supply[i] << "\n";
    }
    cout << "Total Produced: " << total_produced << "\n";
    cout << "Total Exchanged: " << total_exchanged << "\n";
    cout << "Total Money: " << total_money << "\n";
    if (alive == 0)
    {
        cout << "Everyone is dead! \n";
        return;
    }
    cout << "End of log \n \n";

    reset();
    return;
}

void all(string identifier)
{
    if (identifier == "checkin")
    {
        for (int i = 0; i < N; i++)
        {
            if (humano[i].life == 1)
            {
                humano[i].checkin();
            }
        }
        return;
    }
    if (identifier == "checkout")
    {
        for (int i = 0; i < N && humano[i].life == 1; i++)
        {
            if (humano[i].life == 1)
            {
                humano[i].checkout();
            }
        }
        return;
    }
    if (identifier == "production")
    {
        for (int i = 0; i < N && humano[i].life == 1; i++)
        {
            if (humano[i].life == 1)
            {
                humano[i].production();
            }
        }
        return;
    }
    if (identifier == "seek_trade")
    {
        for (int i = 0; i < N && humano[i].life == 1; i++)
        {
            if (humano[i].life == 1)
            {
                humano[i].seek_trade();
            }
        }
        return;
    }
}