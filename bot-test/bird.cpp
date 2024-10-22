#include "bird.h"
#include <iostream>

Bird::Bird(V<float>& weights) {
    assert(weights.size() == 18);
    //  Init input -> hidden
    i_weights = MAT<float>(3,V<float>(5));
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 5; ++j)
            i_weights[i][j] = weights[i*5+j];
    //  Init hidden -> output
    h_weights = V<float>(3);
    for(int i = 15; i < 18; ++i)
        h_weights[i-15] = weights[i];
    //  Sentinel fitness val
    alive = 1;
}

bool Bird::f_prop(V<float>& inputs) {
    float o_sum = 0.0f;
    for(int i = 0; i < 3; ++i) {
        //  Sum for current hidden node
        float h_sum = 0.0f;
        for(int j = 0; j < 5; ++j)
            h_sum += (i_weights[i][j]*inputs[j]);
        //  Pass on sigmoid(hidden node value) * weight to output node
        o_sum += sigmoid(h_sum)*h_weights[i];
    }
    //  Flap at 50% confidence
    return sigmoid(o_sum) >= 0.5f;
}

float Bird::sigmoid(float x) {
    return 1.0f/(1.0f+exp(-x));
}