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

    vector<float> price, max_price, min_price, next_price;
    vector<float> needs_cost;
    vector<bool> trade_sucess;

    int life;

    product job = default_job;
    int id, productivity, productivity_rate;
    int total_production;
    int last_production;
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
        sensitivity = max(float(0.01), sensitivity);
        sensitivity = min(float(1), sensitivity);

        productivity_rate = initial_productivity_rate;

        for (int i = 0; i < G; i++)
        {
            needs.pb(initial_needs[i]);
            priority.pb(setup_priority[i]);

            price.pb(initial_prices[i]);
            max_price.pb(price[i]);
            min_price.pb(price[i]);
            next_price.pb(price[i]);
            needs_cost.pb(0);

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

state brazil;

void human::checkin()
{
    // Prices are updated in the morning, when they are going to be sold
    update_prices();
    last_production = 0;
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
            brazil.money += money;
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
        last_production += job.result;
    }

    update_productivity();
    return;
}

void human::update_productivity()
{
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
    float price_dif = seller.price[good] - buyer.price[good];

    // How much does this trade matter in the total value
    // To Dev: Change to make it compare with traded inteasted of produced
    // This will make trade value be more accurate
    // Also, make the same for the buyer part, with how much he saw in
    // the market last day, to make him change the price based on supply

    // For now only seller is being implemented
    // This min max will return a value from 0 to quantity
    // Trade value represents how much it will be traded in comparisson to
    // the seller production (which should be switched to traded as Dev states above).
    int quantity_to_trade = min(max(seller.dis[good], 0), quantity);
    int trade_value =
        (quantity - min(max(seller.dis[good], 0), quantity) / seller.last_production) *
        (seller.price[good] * seller.sensitivity);
    // Adjusting min and max price of both seller and buyer.
    buyer.max_price[good] = max(buyer.max_price[good], seller.price[good]);
    seller.max_price[good] = max(seller.max_price[good], buyer.price[good]);

    buyer.min_price[good] = min(buyer.min_price[good], seller.price[good]);
    seller.min_price[good] = min(seller.min_price[good], buyer.price[good]);

    // Start to compare quantity and availability of the product by
    // changing prices AND adjusting how much is traded.
    if (seller.dis[good] < quantity && seller.dis[good] > 0)
    {
        // Buyer wants to buy more than Seller has.
        // This will grow seller price proportionally, and the trade will occur
        // with how much the seller has.

        seller.next_price[good] +=
            ((quantity - seller.dis[good]) / seller.last_production) *
            (seller.price[good] * seller.sensitivity);
        quantity = seller.dis[good];
    }
    if (seller.dis[good] <= 0)
    {
        // Seller doesn't have enough goods, thus trade will end, but
        // not before he improves his price based on how much he made.
        seller.next_price[good] +=
            (quantity / seller.last_production) *
            (seller.price[good] * seller.sensitivity);
        quantity = seller.dis[good];
        humankind[id_buyer] = buyer;
        humankind[id_seller] = seller;
        return;
    }
    quantity = min(quantity, seller.dis[good]);
    int price = seller.price[good];

    if (price > buyer.price[good])
    {
        if (buyer.inventory[good] >= buyer.needs[good])
        {
            // If the buyer refuses to buy, both next prices are changed
            buyer.next_price[good] += price_dif * buyer.sensitivity;
            seller.next_price[good] -= price_dif * seller.sensitivity;
            humankind[id_buyer] = buyer;
            humankind[id_seller] = seller;
            cout << "They didn't trade\n";
            cout << "Old values: " << buyer.price[good] << " ";
            cout << seller.price[good] << "\n";
            cout << "New values: " << buyer.next_price[good] << " ";
            cout << seller.next_price[good] << "\n";
            return;
        }
        else
        {
            // If the buyer needs the product to survive, he tries to buy it anyway,
            // changing their perception of how much it's valuable by a smaller amount.
            buyer.next_price[good] += price_dif * buyer.sensitivity * 0.5;
            humankind[id_buyer] = buyer;
            humankind[id_seller] = seller;
        }
    }

    cout << "They traded for the price " << price << "\n";
    cout << "Buyer " << buyer.id << " price is " << buyer.price[good] << "\n";
    cout << "Seller " << seller.id << " price is " << seller.price[good] << "\n";
    buyer.next_price[good] += price_dif * buyer.sensitivity;
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
    // price_different things. The human AI currently prioritizes
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
    for (int good = 0; good < G; good++)
    {
        price[good] = next_price[good];
    }
    return;
}

void human::request_aid(int grant)
{
    int value = brazil.donate(humankind[id], grant);
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