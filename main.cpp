// Main file that creates boomerang animation

/*
USAGE:
    - make build  : Creates exec file
    - make run    : Runs exec file
    - make all    : Create + Run
    - make clean  : Remove exec
*/
#include <SFML/Graphics.hpp>
#include <cmath>  // For sin() and cos()


// Function to compute the normalized perpendicular vector
sf::Vector2f perpendicular(const sf::Vector2f& v) {
    return sf::Vector2f(-v.y, v.x);  // Rotates the vector 90 degrees counter-clockwise
}

// Function to normalize a vector
sf::Vector2f normalize(const sf::Vector2f& v) {
    float length = std::sqrt(v.x * v.x + v.y * v.y);
    if (length != 0) {
        return sf::Vector2f(v.x / length, v.y / length);
    }
    return v;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Boomerang Spiral in SFML");

    window.setFramerateLimit(30);

    // Center of the window
    sf::Vector2f center(400, 400);

    // Parameters for the spiral
    const float a = 6.0f;    // Starting radius
    const float b = 17.0f;     // Spiral expansion rate
    const int numPoints = 104;  // Total number of points for a full spiral
    const float thetaStep = 0.2f;  // Angle step for spacing
    const float thickness = 40.0f;  // Thickness of the spiral line

    // Create a VertexArray to hold the quads (4 vertices per quad)
    sf::VertexArray spiral(sf::Quads, (numPoints - 1) * 4);  // Quads need 4 vertices each

    // Precompute all points of the spiral
    for (int i = 0; i < numPoints - 1; ++i) {
        float theta1 = i * thetaStep;
        float theta2 = (i + 1) * thetaStep;

        // Polar to Cartesian conversion for the current and next point
        float radius1 = a + b * theta1;
        float x1 = radius1 * std::cos(theta1);
        float y1 = radius1 * std::sin(theta1);

        float radius2 = a + b * theta2;
        float x2 = radius2 * std::cos(theta2);
        float y2 = radius2 * std::sin(theta2);

        // Define the current and next points
        sf::Vector2f point1(center.x + x1, center.y + y1);
        sf::Vector2f point2(center.x + x2, center.y + y2);

        // Compute the direction vector and the perpendicular vector for thickness
        sf::Vector2f direction = normalize(point2 - point1);
        sf::Vector2f offset = perpendicular(direction) * (thickness / 2.0f);

        // Set the quad vertices (4 corners of the thick line segment)
        // First two vertices (left side of the line segment)
        spiral[i * 4].position = point1 + offset;   // Upper-left corner
        spiral[i * 4 + 1].position = point2 + offset;  // Upper-right corner

        // Last two vertices (right side of the line segment)
        spiral[i * 4 + 2].position = point2 - offset;  // Lower-right corner
        spiral[i * 4 + 3].position = point1 - offset;  // Lower-left corner

        // Set the color for each vertex (e.g., gradient effect)
        sf::Color color = sf::Color(255, 255 - i % 255, i % 255);  // Gradient color
        spiral[i * 4].color = color;
        spiral[i * 4 + 1].color = color;
        spiral[i * 4 + 2].color = color;
        spiral[i * 4 + 3].color = color;
    }

    // Variables to control the animation
    int visibleSegments = 0;      // Number of currently visible segments
    const int segmentsPerFrame = 2;  // Number of segments to reveal per frame
    bool expanding = true;        // Track whether the animation is expanding or contracting

    sf::Clock clock;  // To keep track of time for smooth animation

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Time-based segment revealing logic
        float deltaTime = clock.getElapsedTime().asSeconds();
        clock.restart();  // Restart the clock for the next frame

        // Boomerang effect: Expand or contract the spiral based on the current direction
        if (expanding) {
            visibleSegments += segmentsPerFrame;  // Reveal more segments
            if (visibleSegments >= (numPoints - 1)) {
                visibleSegments = numPoints - 1;  // Stop at the end
                expanding = false;  // Start contracting
            }
        }
        else {
            visibleSegments -= segmentsPerFrame;  // Hide segments (contract)
            if (visibleSegments <= 0) {
                visibleSegments = 0;  // Stop at the beginning
                expanding = true;  // Start expanding again
            }
        }

        // Clear the window
        window.clear(sf::Color::Black);

        // Draw only the visible part of the spiral (progressively increasing or decreasing)
        window.draw(&spiral[0], visibleSegments * 4, sf::Quads);  // Draw visible quads

        // Display the window contents
        window.display();
    }

    return 0;
}
