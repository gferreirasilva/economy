#include "header.h"

int product_id = 0;

class product
{
public:
    int id;
    int production_cost;
    int result;

    int base_productivity;
    float productivity_rate;
    float max_productivity;

    int weight;
    int expiration_time;
    float people_factor;

    vector<int> materials_needed;
    // To Dev: Fully transport productivity related stuff to products, from entitites.
    product(
        int initial_production_cost,
        int initial_result,
        int initial_weigth,
        float setup_people_factor,
        int setup_expiration_time,
        int setup_base_productivity,
        float setup_productivity_rate,
        float setup_max_productivity,
        vector<int> materials)
    {
        id = product_id;
        product_id++;

        production_cost = initial_production_cost;
        result = initial_result;
        weight = initial_weigth;
        people_factor = setup_people_factor;

        expiration_time = setup_expiration_time;

        base_productivity = setup_base_productivity;
        productivity_rate = setup_productivity_rate;
        max_productivity = setup_max_productivity;

        for (int i = 0; i < materials.size(); i++)
        {
            materials_needed.pb(materials[i]);
        }
    }
};

vector<product> products;
