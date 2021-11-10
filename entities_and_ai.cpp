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
    // default-job stuff
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

    product job = default_job;
    int life, starvation, hunger;

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
        starvation = hungry_limit;
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
        //To Dev
        //Conditions to analyze: How much money do we have? How much money does the human have? What job does the human have?
        //For now all requests to donate money are being accepted.
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

    for (int q = 0; q < G; q++)
    {
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
    update_prices();
    for (int q = 0; q < G; q++)
    {
        if (inventory[q] - needs[q] > starvation)
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
            gov.money += money;
            money = 0;
            return;
        }
        if (inventory[q] > 0)
        {
            int used = min(needs[q], inventory[q]);
            inventory[q] -= used;
            needs[q] -= used;
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
            if (job.materials_needed[i] > inventory[i])
            {
                cout << inventory[i] << " " << job.materials_needed[i] << " alo? \n";
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

    //Adjusting min and max price of both seller and buyer.
    seller.max_price[good] = max(seller.max_price[good], buyer.price[good]);
    seller.min_price[good] = max(seller.min_price[good], buyer.price[good]);
    buyer.max_price[good = max(buyer.max_price[good], seller.price[good])];
    buyer.min_price[good] = max(buyer.min_price[good], seller.price[good]);

    if (seller.dis[good] <= 0)
    {
        // To Dev
        // Change everything here to "adjust prices", which will happen at checkout
        // when people decide if they got what they wanted or not, base on max an
        // min prices they searched.

        // Early interpretation of inflation and loss of consumption power.
        // Maybe it happens when you don't produce it too?
        if (seller.job.id == good)
        {
            if (seller.price[good] == 0)
            {
                seller.price[good] = 1;
                // Make a better min price, based on price of production
                // This "price of production" may be based on what he needs to survive
            }
            seller.price[good] += seller.price[good] * seller.sensitivity * 0.25;
            // How much the price changes if the seller doesn't have enough
            // money is totally arbitrary fow now.
        }

        humankind[id_seller] = seller;
        return;
    }
    quantity = min(quantity, seller.dis[good]);

    int price = seller.price[good];

    if (price > buyer.price[good])
    {

        if (-buyer.dis[good] >= buyer.starvation)
        {
            // People very likely to buy something, at thatever the
            // price currently is, are more likely to change their price
            // when they are obglited to buy.
            buyer.price[good] += dif * buyer.sensitivity * 0.5;
        }

        else
        {
            buyer.max_price[good] = max(buyer.max_price[good], seller.price[good]);
            buyer.price[good] += dif * buyer.sensitivity;
            seller.price[good] -= dif * buyer.sensitivity;
            humankind[id_buyer] = buyer;
            humankind[id_seller] = seller;

            return;
        }
    }

    buyer.inventory[good] += quantity;
    buyer.dis[good] += quantity;

    seller.inventory[good] -= quantity;
    seller.dis[good] -= quantity;

    demand[good] -= quantity;
    supply[good] -= quantity;

    buyer.money -= price;
    seller.money += price;

    total_exchanged += quantity * price;

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
    for (int goods = 0; goods < G; goods++)
    {
        if (trade_sucess[goods])
        {
            price[goods] += sensitivity * (-price[goods] + max_price[goods]);
            continue;
        }
        if (price[goods] > min_price[goods])
        {
            price[goods] -= sensitivity * (+price[goods] - min_price[goods]);
        }
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