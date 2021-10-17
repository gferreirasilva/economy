#include "header.h"

// Weird solution needed because of "default job"
// To Dev
// Find a solution to make product_id start at 0
int product_id = 0;

class product
{
public:
    int id;
    int production_cost;
    int base_production;
    float productivity_rate;
    float max_productivity;
    vector<int> materials_needed;
    int weight;
    int result;
    int expiration_time;
    float people_factor;
    // Initial refers to what may change in the future, setup is defined by nature
    // and can't be changed.
    // What may or not be dynamic is defined on "economy.txt".
    product(
        vector<int> materials,
        int initial_production_cost = 100,
        int setup_base_production = 100,
        float setup_productivity_rate = 1,
        float setup_max_productivity = 1,
        int initial_weight = 1,
        int initial_result = 1,
        int initial_expiration_time = -1,
        float setup_people_factor = 1)
    {
        for (int i = 0; i < materials.size(); i++)
        {
            materials_needed.pb(materials[i]);
        }
        id = product_id;
        product_id++;
        production_cost = initial_production_cost;
        base_production = setup_base_production;
        productivity_rate = setup_productivity_rate;
        max_productivity = setup_max_productivity;
        weight = initial_weight;
        expiration_time = initial_expiration_time;
        result = initial_result;
        people_factor = setup_people_factor;
    }
};

vector<product> products;
