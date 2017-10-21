// Hand-in 5 in the course Mathematics for Programming
// Thomas Løkkeborg 
// thomahl@stud.ntnu.no
//
// Visualization of A* algorithm on 32x32 grid.

#include <iostream>
#include <algorithm>
#include <set>
#include <queue>
#include <SFML/Graphics.hpp>
#include <array>


//------------------------------ UTILITY FUNCTION ------------------------------

// Returns absolute value of an int (forces positive)
int abs(int num) {
    if (num < 0)
        num *= -1;
    return num;
}

//--------------------------------- TILE CLASS ---------------------------------

// A single tile in the Tilemap. Stores g, h -values and x, y -coordinates 
//  internally for simplicity. (g set to a large number to imitate infinity)
struct Tile {
    std::size_t x = 0, y = 0;       // Position in grid (used to find neighbors)
    float g = 999999, h = 0;        // g: dist from goal, h: heuristic

    bool traversable = true;        // is this tile traverable?
    bool inOpen = false;            // is this tile in the open list?
    bool inClosed = false;          // is this tile in the closed list?

    Tile* parent = nullptr;         // parent is previous tile in path to goal

    float calcF() const {           // returns the F value. sum of g and h
        return g + h;
    }
};

//------------------------------- TILEMAP CLASS --------------------------------

// 32x32 grid of tiles along with operations on the grid. Performs A* algorithm 
//  and visualizes the algorithm on the grid using color-coding
class Tilemap {
    private:
        std::array<std::array<Tile, 32>, 32> tiles; // the grid
        Tile* start;                                // p to start tile
        Tile* end;                                  // p to goal tile
        const int screenWidth, screenHeight;        // width, height of screen
        const int tileWidth, tileHeight;            // width, height of tiles

        sf::RectangleShape tileShape;               // shape to use for drawing

        // colors: (static, initialized below class-decl)
        static const sf::Color unvisitedColor;  // color of unvisited tile
        static const sf::Color inOpenColor;     // color of tile in open list
        static const sf::Color inClosedColor;   // color of tile in closed list
        static const sf::Color nonTravColor;    // color of non-traversable tile
        static const sf::Color startColor;      // color of start-tile
        static const sf::Color endColor;        // color of goal-tile
        static const sf::Color pathColor;       // color of tile in path to goal

        // Cleans up visualization data from previous A* 
        void cleanup() {
            for (std::size_t i = 0; i < 32; ++i) {
                for (std::size_t j = 0; j < 32; ++j) {
                    tiles[i][j].inOpen = false;         // resets inOpen
                    tiles[i][j].inClosed = false;       // resets inClosed
                    tiles[i][j].parent = nullptr;       // resets parent
                }
            } 
        }

        // Returns neighbors of 'tile' in the grid in vector. A tile potentially
        //  has four neighbors: (x-1, y), (x+1, y), (x, y-1), (x, y+1). The 
        //  function checks for out-of-bounds and returns these
        //  TODO adding const to function causes problems... why??
        std::vector<Tile*> neighborsOf(const Tile* const tile) {

            std::vector<Tile*> neighbors;       // vector of neighbors

            if (tile->x > 0)        // has neighbor to the left?
                neighbors.push_back(&tiles[tile->x-1][tile->y]);

            if (tile->x < 31)       // has neighbor to the right?
                neighbors.push_back(&tiles[tile->x+1][tile->y]);

            if (tile->y > 0)        // has neighbor above?
                neighbors.push_back(&tiles[tile->x][tile->y-1]);

            if (tile->y < 31)       // has neighbor below?
                neighbors.push_back(&tiles[tile->x][tile->y+1]);

            return neighbors;
        }

        // Attempts to reconstruct path found by A* and return it in a vector.
        //  If no path exists: returns empty vector
        std::vector<Tile*> reconstructPath() const {

            std::vector<Tile*> path;        // vector to store path in 

            Tile* tile = end;               // tile: tmp variable. set to end.
            while (tile != start) {         // as long as haven't reached start.
                path.push_back(tile);       // add tile to path.
                if (tile->parent) {         // check if parent exists.
                    tile = tile->parent;    // IF YES: set tile to parent.
                } else {                    // IF NO: return empty vector.
                    return std::vector<Tile*>();    // (give up).
                }
            }

            return path;        // path found. return it
        }

        // Attempt to find path to goal using A*. Updates tile's parent, inOpen 
        //  and inClosed properties for visualization. NB: the std::vector open 
        // is used as a heap by applying fancy STL functions (std::make_heap, 
        //  std::pop_heap, std::push_heap).
        void doAStar() {
            
            // lambda used in sorting of heap
            auto tilePCmp = [](const Tile* t1, const Tile* t2) 
                    { return t1->calcF() > t2->calcF(); };

            // Set start tile's g-value to 0. (others set to INF in constructor)
            start->g = 0;

            // vector of tiles to inspect (NB: used as heap)
            std::vector<Tile*> open;    

            open.push_back(start);      // add starting node
            std::make_heap(open.begin(), open.end(), tilePCmp);  // heapify

            Tile* current = nullptr;    // the tile we are currently inspecting

            // Run algorithm until goal is reached OR there are no more tiles 
            //  to inspect (no path)
            while (current != end && !open.empty()) {

                // tile with lowest f-value moved from open to current:

                // (moves tile with lowest f-value to back of vector first)
                std::pop_heap(open.begin(), open.end(), tilePCmp);
                
                current = open.back(); // set current to tile with lowest f-value
                open.pop_back();       // remove current tile from open
                current->inClosed = true;    // indicate that it has been visited

                // For each neighbor of current: Discover / Attempt to update 
                for (Tile* nb : neighborsOf(current)) {

                    // if neibhor is non-traversable: ignore it
                    if (!nb->traversable) {
                        continue;
                    }

                    // if neighbor has been visited: ignore it
                    if (nb->inClosed) {
                        continue;
                    }

                    // Calculate g-value of moving to neighbor from current node
                    float tentativeG = current->g + 1; // movecost set to 1

                    // if we haven't seen this tile before, add it to open
                    if (!nb->inOpen) {

                        // update parent and g-value of neighbor
                        nb->parent = current;
                        nb->g = tentativeG;

                        nb->inOpen = true; // indicate it is in open (for color)

                        open.push_back(nb);     // add neighbor to open

                        // sort the new element into open (heapify)
                        push_heap(open.begin(), open.end(), tilePCmp);

                    // if new g-value isn't better than the old one: do nothing
                    } else if (tentativeG < nb->g) {
                        // update parent and g-value of neighbor
                        nb->parent = current;
                        nb->g = tentativeG;

                        // heapify
                        std::make_heap(open.begin(), open.end(), tilePCmp);
                    }
                }
            }
        }

    public:

        // Constructs object based on size of screen
        Tilemap(int scrnW, int scrnH): screenWidth(scrnW), screenHeight(scrnH), 
                tileWidth(scrnW/32), tileHeight(scrnH/32), 
                tileShape(sf::Vector2f(tileWidth, tileHeight)) {

            // sets start and end to be at the top and bottom. middle of screen
            setStart(screenWidth/2, 0);
            setEnd(screenWidth/2,screenHeight-1);

            // let's tiles know their (x, y)-coordinates in the grid
            for (std::size_t i = 0; i < 32; ++i) {
                for (std::size_t j = 0; j < 32; ++j) {
                    tiles[i][j].x = i;
                    tiles[i][j].y = j;
                }
            }

            // sets up the tileShape
            tileShape.setOutlineColor(sf::Color::Black);
            tileShape.setOutlineThickness(1);
        }

        // Set position of start-tile. Input are screen-coordinates
        void setStart(float screenX, float screenY) {
            int tileX = (screenX / screenWidth) * 32;   // converts to grid
            int tileY = (screenY / screenHeight) * 32;  // -coordinates

            start = &tiles[tileX][tileY];   // sets start
        }

        // Set position of end-tile. Input are screen-coordinates
        void setEnd(float screenX, float screenY) {
            int tileX = (screenX / screenWidth) * 32;   // converts to grid
            int tileY = (screenY / screenHeight) * 32;  // -coordinates
             
            end = &tiles[tileX][tileY];     // sets end

            // updates heuristic to Manhattan Distance from end
            for (std::size_t i = 0; i < 32; ++i) {
                for (std::size_t j = 0; j < 32; ++j) {
                    tiles[i][j].h = abs(i-tileX) + abs(j-tileY);
                }
            }
        }

        // updates the Tilemap
        void update() {
            cleanup();      // cleans up from previous update
            doAStar();      // does A* again
        }

        // draw the tilemap to the screen with colorcoding based on A*
        void draw(sf::RenderTarget& rt) {

            // draws all tiles with colorcoding
            for (std::size_t i = 0; i < 32; ++i) {
                for (std::size_t j = 0; j < 32; ++j) {
                    if (!tiles[i][j].traversable) {     
                        tileShape.setFillColor(nonTravColor);
                    } else if (tiles[i][j].inClosed) {
                        tileShape.setFillColor(inClosedColor);
                    } else if (tiles[i][j].inOpen) {
                        tileShape.setFillColor(inOpenColor);
                    } else {
                        tileShape.setFillColor(unvisitedColor);
                    }
                    tileShape.setPosition(2+tileWidth*i, 2+tileHeight*j); 
                    rt.draw(tileShape);
                }
            } 

            // colors tiles in path (does nothing if no path)
            for (const Tile* tile : reconstructPath()) {
                tileShape.setPosition(2+tileWidth*tile->x, 
                        2+tileHeight*tile->y);
                tileShape.setFillColor(pathColor);
                rt.draw(tileShape);
            }

            // colors start-tile
            tileShape.setPosition(2+tileWidth*start->x, 2+tileHeight*start->y);
            tileShape.setFillColor(startColor);
            rt.draw(tileShape);

            // colors goal-tile
            tileShape.setPosition(2+tileWidth*end->x, 2+tileHeight*end->y);
            tileShape.setFillColor(endColor);
            rt.draw(tileShape);
        }

        // Toggles the traverableness of tile. Input are screen-coordinates
        void toggleTile(float screenX, float screenY) {
            int tileX = (screenX / screenWidth) * 32;    // converts to grid
            int tileY = (screenY / screenHeight) * 32;   // -coordinates

            // toggles traversableness using fancy ternary operator 
            tiles[tileX][tileY].traversable = 
                    (tiles[tileX][tileY].traversable ? false : true);
        }
};

// initializers for static members
const sf::Color Tilemap::unvisitedColor(sf::Color::White);           
const sf::Color Tilemap::inOpenColor(sf::Color(200,200,200));           
const sf::Color Tilemap::inClosedColor(sf::Color(150,150,150));
const sf::Color Tilemap::nonTravColor(sf::Color::Black);
const sf::Color Tilemap::startColor(sf::Color::Green);
const sf::Color Tilemap::endColor(sf::Color::Red);
const sf::Color Tilemap::pathColor(sf::Color::Magenta);

//------------------------------------ MAIN ------------------------------------

int main() {

    const int WINSIZE = 900;     // width, height of window

    // set up RenderWindow
    sf::RenderWindow window(sf::VideoMode(WINSIZE, WINSIZE), "A*");

    // set up Tilemap
    Tilemap tmap(900, 900);

    // run the program as long as the window is open
    while (window.isOpen()) {

        sf::Event event;    // poll events

        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            // left-mouseclick toggles traversablesness of tile, 
            //  right-mouseclick sets position of end tile
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    tmap.toggleTile(event.mouseButton.x, event.mouseButton.y);
                } else {
                    tmap.setEnd(event.mouseButton.x, event.mouseButton.y);
                }
            }
        }

        tmap.update();                              // update tmap
        window.clear(sf::Color::Black);             // clear window
        tmap.draw(window);                          // draw tmap
        window.display();                           // display window
    }

    return 0;
} 
