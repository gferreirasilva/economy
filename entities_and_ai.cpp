#include "header.h"
#include "products.cpp"

typedef pair<int, int> pii;

int N = 0;
int G = 0;
int total_exchanged = 0;
int total_money = 0;
int total_produced = 0;
int human_id = 0;
int alive = 0;

int max_productivity;

vector<int> supply, demand;
vector<int> goods_zero;
product default_job(goods_zero);

void init(int n, int g)
{
    N = n;
    G = g;

    alive = N;

    for (int i = 0; i < G; i++)
    {
        goods_zero.pb(0);
    }
    demand = supply = goods_zero;
    // default job stuff
    product new_job(goods_zero);
    default_job = new_job;
    products.clear();
    product_id = 0;
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
    vector<int> needs, priority, inventory, dis, security;

    vector<float> price, max_price, min_price;
    vector<bool> trade_sucess;

    int life;

    product job = default_job;
    int id, productivity, productivity_rate;
    int total_production;
    float money, sensitivity, income;

    human(
        float initial_money,
        int hungry_limit,
        int initial_job_id,
        float initial_sensitivity,
        float initial_productivity_rate,
        vector<int> initial_needs,
        vector<float> initial_prices,
        vector<int> setup_priority = goods_zero,
        vector<int> initial_inventory = goods_zero,
        vector<int> initial_security = goods_zero)
    {
        id = human_id;
        human_id++;
        money = initial_money;
        job = products[initial_job_id];
        productivity = job.base_production;
        sensitivity = initial_sensitivity;
        productivity_rate = initial_productivity_rate;

        for (int i = 0; i < G; i++)
        {
            needs.pb(initial_needs[i]);
            priority.pb(setup_priority[i]);

            price.pb(initial_prices[i]);
            max_price.pb(price[i]);
            min_price.pb(price[i]);
            trade_sucess.pb(false);

            inventory.pb(initial_inventory[i]);
            security.pb(max(initial_security[i], initial_needs[i]));
            // Security has to be at least needs.
            dis.pb(0);
        }
        life = 1;
        total_production = 0;
    }

    void checkin();
    void checkout();
    void seek_trade();
    void production();
    void update_productivity();
    void update_prices();
    void change_job();
    void request_aid(int cash);
};

vector<human> humankind;

class state
{
public:
    vector<int> credit, debt;
    int money;
    float fees;

    state(int dinheiro = 100, float taxas = 0)
    {
        money = dinheiro;
        fees = taxas;
    }

    void tax()
    {
        int taxation;
        int initial_money = money;
        for (int i = 0; i < N; i++)
        {
            taxation = humankind[i].money * fees;
            money += taxation;
            humankind[i].money -= taxation;
        }
        cout << money - initial_money << " was taxed";
        return;
    }
    void print_money(int cash)
    {
        // To Dev
        // Conditions to analyze: Will this money encourage productivity
        // in valuable sectors of the economy?
        // For now all requests for generating money are being accepted.
        money += cash;
        cout << "Printed " << cash << " money \n";
        return;
    }
    void buy()
    {
        //To Dev
        return;
    }
    int donate(human asker, int grant)
    {
        // To Dev
        // Conditions to analyze: How much money do we have?
        // How much money does the human have? What job does the human have?
        // For now all requests to donate money are being accepted.
        if (money >= grant)
        {
            money -= grant;
            return grant;
        }
        if (money < grant)
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
    void invest()
    {
        //To Dev
        return;
    }
};

state gov;

void human::checkin()
{
    // Prices are updated in the morning, when they are going to be sold
    update_prices();
    for (int q = 0; q < G; q++)
    {
        trade_sucess[q] = false;
        dis[q] = inventory[q] - security[q];

        // Preparing log
        if (dis[q] < 0)
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
        if (needs[q] > inventory[q])
        {

            request_aid(max_price[q]);
            if (max_price[q] <= money)
            {
                seek_trade();
            }
        }

        if (needs[q] > inventory[q])
        {
            life = 0;
            alive--;
            cout << id << " died\n";
            gov.money += money;
            money = 0;
            return;
        }
        if (inventory[q] > needs[q])
        {
            inventory[q] -= needs[q];
        }
    }
    total_money += money;
    return;
}

void human::production()
{
    total_production += productivity;
    while (total_production >= job.production_cost)
    {

        for (int i = 0; i < G; i++)
        {
            if (job.materials[i] > inventory[i])
            {
                cout << inventory[i] << " " << job.materials[i] << " alo? \n";
                total_production -= productivity;
                return;
            }
        }
        total_production -= job.production_cost;
        inventory[job.id] += job.result;
        total_produced += price[job.id] * job.result;
    }

    update_productivity();
    return;
}

void human::update_productivity()
{
    // To Dev
    // For now productivity growth is linear, but it's easy to implement logarithmic growth

    productivity += job.productivity_rate * job.base_production * productivity_rate;
    productivity = min(
        int(job.base_production * job.max_productivity),
        productivity);
    return;
}

void trade(int id_buyer, int id_seller, int quantity, int good)
{
    human buyer = humankind[id_buyer];
    human seller = humankind[id_seller];
    int dif = seller.price[good] - buyer.price[good];
    cout << buyer.price[good] << " and " << seller.price[good] << "\n";
    if (seller.dis[good] < 0)
    {
        // Seller doesn't have enough goods
        return;
    }
    // Adjusting min and max price of both seller and buyer.
    buyer.max_price[good] = max(buyer.max_price[good], seller.price[good]);
    seller.max_price[good] = max(seller.max_price[good], buyer.price[good]);

    buyer.min_price[good] = min(buyer.min_price[good], seller.price[good]);
    seller.min_price[good] = min(seller.min_price[good], buyer.price[good]);

    quantity = min(quantity, seller.dis[good]);
    int price = seller.price[good];

    if (price > buyer.price[good])
    {

        if (buyer.inventory[good] >= buyer.needs[good])
        {
            humankind[id_buyer] = buyer;
            humankind[id_seller] = seller;
            return;
        }
    }

    buyer.trade_sucess[good] = true;
    seller.trade_sucess[good] = true;

    buyer.inventory[good] += quantity;
    seller.inventory[good] -= quantity;

    buyer.dis[good] += quantity;
    seller.dis[good] -= quantity;

    buyer.money -= price;
    seller.money += price;

    total_exchanged += (quantity * price);

    humankind[id_buyer] = buyer;
    humankind[id_seller] = seller;
    return;
};

void human::seek_trade()
{
    // Code defining when the AI will decide to buy
    // different things. The human AI currently prioritizes
    // thing that are in a setup priority, and doesn't evaluete
    // if the priority makes sense or if it's benefical.
    // To Dev
    // Need to implement a seek_trade in which the seller seeks the buyer.
    // Only the buyer seeking the seller is currently available.
    for (int item = 0; item < G; item++)
    {
        int good = priority[item];

        if (dis[good] >= 0)
        {
            continue;
        }

        for (int k = 0; k < N; k++)
        {
            if (id == k || humankind[k].life == 0)
            {
                continue;
            }
            trade(id, k, -dis[good], good);
        }
    }
}
void human::update_prices()
{
    cout << "For human " << id << ": \n";
    for (int good = 0; good < G; good++)
    {
        int before = price[good];
        if (trade_sucess[good] == true)
        {
            price[good] += sensitivity * (-price[good] + max_price[good]);
        }
        price[good] -= sensitivity * (price[good] + min_price[good]);

        cout << "Change in price of product " << good << " : " << price[good] - before << "\n";
    }
    return;
}

void human::request_aid(int grant)
{
    int value = gov.donate(humankind[id], grant);
    money += value;
    return;
}

void log()
{
    for (int i = 0; i < G; i++)
    {
        cout << "Product: " << i << ": ";
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
            if (humankind[i].life == 1)
            {
                humankind[i].checkin();
            }
        }
        return;
    }
    if (identifier == "checkout")
    {
        for (int i = 0; i < N && humankind[i].life == 1; i++)
        {
            if (humankind[i].life == 1)
            {
                humankind[i].checkout();
            }
        }
        return;
    }
    if (identifier == "production")
    {
        for (int i = 0; i < N && humankind[i].life == 1; i++)
        {
            if (humankind[i].life == 1)
            {
                humankind[i].production();
            }
        }
        return;
    }
    if (identifier == "seek_trade")
    {
        for (int i = 0; i < N && humankind[i].life == 1; i++)
        {
            if (humankind[i].life == 1)
            {
                humankind[i].seek_trade();
            }
        }
        return;
    }
}