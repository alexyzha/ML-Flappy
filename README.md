# Using a genetic algorithm to train neural networks to play flappy bird:

**Over 10,000 columns passed (click for video):**

[![thumbnail](visuals/thumbnail.png)](https://youtu.be/TEs496ZzPow)

# Overall program structure:

For both the training phase, the overall program is split into 3 parts. The testing phase only has parts 1 and 2 (below):

1. The neural network, or the "bird"
2. The game itself (flappy bird)
3. The genetic algorithm, which is used to evolve birds

# Neural network structure:

Since flappy bird is a pretty simple game, a single hidden layer is good enough. The neural networks I used take in `5` inputs: `bird y-velocity`, `bird y-position`, `next pillar pair x-position`, and finally `next pillar pair top/bottom y-positions`. Since the bird stays in place (constant bird x-value), there isn't a need to use that as an input. The output at a specific state is whether or not the bird should flap, so we have a boolean output. Therefore:

- The neural network's input layer consists of `5` nodes
- The hidden layer consists of `3` nodes (`_5/2_+1=3` was a starting estimate for node count) all using sigmoid as the activation function
- The output layer consists of `1` node using sigmoid as its activation function

Sigmoid was used because the inputs from the game are all fairly large (range = [`0`,`~400`]) and because the final output should range from `0` to `1`. Since this is a fully connected neural network, there will be `5` (input node count) weights in every hidden node and `3` (hidden node count) weights in the output node. This means that there will be `5*3+3*1=18` total weights.

A `Bird` struct contains weights and a variable `fitness` (in training phase, `alive` in testing phase). `f_prop` (forward propagation) is the function used to determine the bird's decision at a certain state. `f_prop` values of `0.5f` or more cause the bird to flap.

To flap or not to flap? 💀

# Genetic algorithm structure:

Each bird has `18` different weights. There are no biases. Therefore, a bird's chromosome consists of `18` floats. These are initially randomly generated using a normal distribution with `mean = 0`, `stdev = 0.1f`. Subsequent singleton mutations are also generated using this distribution. The population size used was `30`, and the birds were trained over `50` generations.

During the training phase, in a generation of birds, the `evolution handler` runs the game until all the birds are dead, or until a certain threshold (`~200` pillars passed) is reached. Each bird is assigned a fitness at the time of death or at the game's conclusion. Then, all birds are sorted by `highest fitness`. Here, fitness is analagous with "time lived."

From the current generation of birds, the top `10%` of birds (by fitness) move on to the next generation. Next, `40%` of the next generation is created using random mating from the top `20%` of birds in the current generation. They mate using a mutation rate of `20%`, and an equal chance (`40%`) of inheriting from either parent. Finally, the last `50%` of the next generation is created using random mating from the top `40%` of birds in the current generation. They mate using a mutation rate of `40%`, and an equal chance (`30%`) of inheriting from either parent.

Generally, after around generation `20`, at least `1` bird was able to 'pass' the simulation (make it to the threshold). For the next `30` or so generations, more birds eventually evolved into being able to pass the simulation.

The simulation was played at a speed of `delta time * 3.0f`, which is about `1.5f` times faster than the video demo, and `3.0f` times faster than the base game. The birds are able to play the game accurately at speeds up to `5.0f` (tested), but everything above `5.0f` times faster than the base game is untested. At the end of `50` generations, the genes of all birds are saved in an output file to be used in the testing phase.

# Game structure:

It's just a no-graphics version of flappy bird. The bird and pipes all have square hitboxs. The opening distance between pipes is fixed, but the opening location is not. The distance between pipes can also vary. Pipes are generated dynamically using c++'s `rand()`.

The main game loop is as follows:

1. `float d_time = clock.restart().asSeconds()*speed` multiply delta time by a factor (user defined)
2. `get_next_pillar()` get the next target pillar pair the birds are using to base their decisions off of
3. `get_inputs()` gets every bird in the simulation's input at their current states
4. `move_birds(d_time)` moves all living birds based on delta time
5. `move_pillars(d_time)` moves all pillars based on delta time
6. `kill_birds()` kills any birds if they are off screen or if they are touching a pillar

# Testing phase:

The testing phase just runs a slightly modified version of the game with a slightly modified version of `Bird` (`float fitness` -> `bool alive`). The `evolution handler` is not used, and lines showing "bird vision" can be seen. From the video, this is what each line color means:

- `Green`: current y-velocity
- `Blue`: distance to bottom pillar
- `Red`: distance to top pillar