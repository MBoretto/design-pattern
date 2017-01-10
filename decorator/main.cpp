#include <vector>
#include <iostream>
#include <string>
#include <functional>
#include<algorithm>

typedef std::vector<std::string> Container;
typedef std::function<Container (Container)> Closure;

class Layer
{
    public:
        virtual Container peel(Container container, Closure next) = 0;
};

class BeforeLayer: public Layer
{
    public:
        Container peel(Container container, Closure next)
        {
            std::string who_am_i = "Before";
            container.push_back(who_am_i);

            return next(container);
        }
};

class AfterLayer: public Layer
{
    public:
        Container peel(Container container, Closure next)
        {
            container = next(container);
            std::string who_am_i = "After";
            container.push_back(who_am_i);

            return container;
        }
};


class Onion
{
    public:
        Onion() {
        }
        //Onion addLayers(std::vector<Layer>) {
        //
        //}

        Container peel(Container& container, Closure closure) {

            std::vector<Layer*> layers;
            layers.emplace_back(new BeforeLayer);
            layers.emplace_back(new AfterLayer);
            layers.emplace_back(new BeforeLayer);
            layers.emplace_back(new AfterLayer);
            //BeforeLayer* layer = new BeforeLayer;
            //layers.push_back(layer);

            Closure c = std::accumulate(layers.begin(), layers.end(),
               this->createCoreFunction(closure), // start with first element
               [this] (Closure next_layer, Layer* layer) {
                   return this->createLayer(next_layer, layer);
               });

            return c(container);
        }

        Closure createCoreFunction(Closure closure) {
            return [closure] (Container container){
                return closure(container);
            };
        }

        Closure createLayer(Closure next_layer, Layer* layer) {
            return [layer, next_layer] (Container container) {
                return layer->peel(container, next_layer);
            };
        }
};

int main() {
    std::vector<std::string> container;

    Onion onion = Onion();

    //onion->addLayers(layers)->peel(container, [&] (std::vector<std::string> container) {
    container = onion.peel(container, [&] (Container container) {
        std::string who_am_i = "Core";
        container.push_back(who_am_i);
        return container;
    });

    for (auto value: container) {
        std::cout<<value<<std::endl;
    }
    return 1;
}
