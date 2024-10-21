#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>

/*
 *  Matrix typing convenience
 */
#ifndef MAT_MACRO
#define MAT_MACRO
#define vcA template<class A
vcA> using V = std::vector<A>;
vcA> using MAT = V<V<A>>;
#endif

class Bird {
public:
    /*
     *  Constructor:
     *  - ONLY takes a vector<float> of size 18 for input
     *  - 18 weights, init with those
     */
    Bird(V<float>& weights);

    /*
     *  Forward propagation:
     *  - Calculates flap/no-flap based on 5 inputs
     *  - x-dist to next pillars
     *  - y-dist to next top pillar
     *  - y-dist to next bottom pillar
     *  - y-velocity
     *  - y-position
     */
    bool f_prop(V<float>& inputs);
    
    /*
     *  Input -> hidden layer:
     *  - Each hidden node has 5 distinct weights
     *  - MAT[i][j], where i = hidden node, j = input node
     */
    MAT<float> i_weights;

    /*
     *  Hidden -> output layer:
     *  - Only 3 weights needed
     */
    V<float> h_weights;

    /*
     *  - Init as -1
     *  - Updated when bird dies or when win condition met
     */
    float fitness;

private:
    /*
     *  Activation function
     *  - 1/(1+e^-x)
     *  - cmath exp(x) = e^x
     */
    float sigmoid(float x);
    
};

/*
    input
    X   hidden
    X   X   output
    X   X   X
    X   X
    X
*/