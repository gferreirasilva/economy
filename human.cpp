#include "header.h"

typedef pair<int, int> pii;

int N = 0;
int G = 0;
int total_exchanged = 0;
int total_money = 0;
int total_produced = 0;
int last_id = 0;
int vivos = 0;

vector<int> supply,demand;

void init(int n, int g){
    N = n;
    G = g;
    vivos = N;

    for(int i = 0; i < G; i++){
        supply.pb(0);
        demand.pb(0);

    }
}

void reset(){
     for (int t = 0; t < G; t++){
        supply[t] = 0;
        demand[t] = 0;
    }
    total_produced = 0;
    total_exchanged = 0;
    total_money = 0;
    
    return;
}

class human{
    public:

    vector<int> needs, prio, inv, nec;
    vector<float> price;

    int life = 1;
    int starvation = 0;

    int id, prod, job;
    float money, sway, income;

    human(int rg , float dinheiro, int fome, int producao, float deslize, int produtividade, vector<int> a, vector<int> b, vector<float> c, vector<int> d){
        id = rg;
        money = dinheiro;
        prod = produtividade;
        job = producao;
        sway = deslize;

        for (int i = 0; i < G; i++){
            needs.pb(a[i]);
            prio.pb(b[i]);
            price.pb(c[i]);
            inv.pb(d[i]);
            nec.pb(0);
        }
        last_id++;
    }

    void checkin();
    void checkout();
    void seek_trade();
    void produce();
    void request_aid();
};

vector<human> humano;

class state{
    public:

    vector<int> credit, debt;
    int money;
    float fees;

    state(int dinheiro, float taxas){
        money = dinheiro;
        fees = taxas;
    }

    void tax(){
        int taxation;
        for (int i = 0; i < N; i++){
            taxation = humano[i].money*fees;
            money += taxation;
            humano[i].money -= taxation;
        }
        return;
    }
    void print_money(){
    
        return;
    }
    void buy(){
    
        return;
    }
    void donate(){
    
        return;
    }
    void lend(){
    
        return;
    }
    void invest(){

        return;
    }
};

void human::checkin(){
    income = 0;
    total_money += money;
    income += price[job]*prod;

    for (int q = 0; q < G; q++){
        nec[q] += inv[q] - needs[q];
        
        if (nec[q] <= 0){
            demand[q] -= nec[q];
        }
        if (nec[q] > 0){
            supply[q] += nec[q];
        }

    }
    
return;
};
void human::checkout(){
    for (int q = 0; q < G; q++){

        if (-nec[q] > starvation){
            request_aid();
        }
        if (-nec[q] > starvation){
        life = 0;
        vivos--;
        cout << id << " morreu\n";
        return;
        }
    }


return;
}

void city_growth(){
    //if ()

    return;
}

void trade(int id_buyer, int id_seller, int produto, int quantidade){
    human buyer = humano[id_buyer];
    human seller = humano[id_seller];

    quantidade = min(quantidade, -buyer.nec[produto]);


    int price = seller.price[produto];

    if (seller.price[produto] > buyer.price[produto]){

        int dif = seller.price[produto] - buyer.price[produto];
        if (-buyer.nec[produto] >= buyer.starvation){
            price = seller.price[produto];
            buyer.price[produto] += dif*buyer.sway;
            
            
        }
        else {

            buyer.price[produto] += dif*buyer.sway;
            seller.price[produto] -= dif*buyer.sway;
            humano[id_buyer] = buyer;
            humano[id_seller] = seller;

            return;
        }
    }

    buyer.inv[produto] += quantidade;
    seller.inv[produto] -= quantidade;

    buyer.money -= price;
    seller.money += price;

    buyer.nec[produto] += quantidade;
    seller.nec[produto] -= quantidade;

    total_exchanged += quantidade*price;

    humano[id_buyer] = buyer;
    humano[id_seller] = seller;
};

void human::produce(){
    inv[job] += prod;
    total_produced += price[job]*prod;

    return;
}

void human::seek_trade(){
    
    for (int item = 0; item < G; item++){
        int it = prio[item];

        if (nec[it] > 0){
            continue;
        }

        for (int k = 0; k < N; k++){
            if (id == k || humano[k].life == 0){continue;}

            if (humano[k].nec[it] > 0){
                trade(id, k, it, humano[k].nec[it]);
            }
        }
    }
}

float att_prod(float prod_total, float prod_rate){
    prod_total += prod_rate;
    if (prod_total >= 1){
        int atual = prod_total;
        prod_total = 0;

        for(int i = 0; i < N; i++){
            humano[i].prod += atual;
        }
        return 0;
    }
    return prod_total;
}


void log(){
    for (int i = 0; i < G; i++){
        cout << "Produto: " << i;
        cout << " Demand: " << demand[i];
        cout << " Supply: " << supply[i] << "\n";
    }
    cout << "Total Produced: " << total_produced << "\n";
    cout << "Total Exchanged: " << total_exchanged << "\n";
    cout << "Total Money: " << total_money << "\n";
    if (vivos == 0){cout << "Everyone is dead! \n"; return;}
    cout << "End of log \n \n";

    reset();
    return;
}


void all(string identifier){
    if (identifier == "checkin"){
        for (int i = 0; i < N; i++){
            if (humano[i].life == 1){
                humano[i].checkin();
            }
        }
        return;
    }
    if (identifier == "checkout"){
        for (int i = 0; i < N && humano[i].life == 1; i++){
            if (humano[i].life == 1){
                humano[i].checkout();
            }
        }
        return;
    }
    if (identifier == "produce"){
        for (int i = 0; i < N && humano[i].life == 1; i++){
            if (humano[i].life == 1){
                humano[i].produce();
            }
        }
        return;
    }
    if (identifier == "seek_trade"){
        for (int i = 0; i < N && humano[i].life == 1; i++){
            if (humano[i].life == 1){
                humano[i].seek_trade(); 
            }
        }
        return;
    }
}