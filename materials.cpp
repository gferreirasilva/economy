#include "header.h"

class product
{
    int production_cost;
    int production_result;
    int weigth;
    // Base price is used to know what is the universal price of a product
    // It doesn't exist in real life, since the value of a product
    // is based on the difficulty and necessity of that item, but will be used
    // on this context to know the initial universal value.
    int base_price;

    vector<int> materials_needed;
    product(int initial_production_cost, int initial_result, int initial_weigth, int initial_base_price, vector<int> materials){
        
        production_cost = initial_production_cost;
        production_result = initial_result;
        base_price = initial_base_price;
        weigth = initial_weigth;

        for (int i = 0; i < materials.size(); i++){
            materials_needed.pb(materials[i]);
        }
    }
};