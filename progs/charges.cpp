#include <iostream>
#include<SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <tuple>

using namespace sf;
const unsigned char CHARGE_POSITIVE = 1;
const unsigned char CHARGE_NEGATIVE = 0;
const unsigned char POINT_SIZE = 5;
const unsigned int WIDTH = 1440;
const unsigned int HEIGHT = 900;

unsigned char mode = CHARGE_POSITIVE;


void drawForceLine(const std::vector<CircleShape> &charges,
                   std::vector<Vertex> &forceLine1,
                   std::vector<Vertex> &forceLine2,
                   Vector2f startPos);

void addCharge(std::vector<CircleShape> &charges, Vector2f pos);

bool inArea(Vector2f v)
{
    return (0 < v.x) && (v.x < WIDTH) &&
            (0 < v.y) && (v.y < HEIGHT);
}

Vector2f normalize(const Vector2f &v)
{
    float len = hypotf(v.x, v.y);
    return v / len;
}

float len2(const Vector2f &v)
{
    return v.x * v.x + v.y * v.y;
}

int main()
{
    std::vector<CircleShape> charges;
    std::vector<std::vector<Vertex>> forceLines;

    RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Window my");
    window.clear((Color::White));

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                     window.close();
                break;
                case Event::MouseButtonPressed:
                    if (Mouse::isButtonPressed(Mouse::Left))
                    {
                        Vector2f pos = static_cast<Vector2f>(Mouse::getPosition(window));
                        forceLines.push_back({Vertex({pos}, Color::Black)});
                        forceLines.push_back({Vertex({pos}, Color::Black)});
                        int size = forceLines.size();
                        drawForceLine(charges, forceLines[size - 1], forceLines[size - 2], pos);
                    }
                    else if (Mouse::isButtonPressed((Mouse::Right)))
                    {
                        Vector2f pos = static_cast<Vector2f>(Mouse::getPosition(window));
                        addCharge(charges, pos);
                    }
                break;

                case Event::KeyPressed:
                    if (Keyboard::isKeyPressed(Keyboard::Space))
                        mode = ~mode;
                break;

            default:
                break;
            }

            window.clear(Color::White);
            for (CircleShape &circle : charges)
                window.draw(circle);
            for (auto forceLine : forceLines)
                window.draw(&forceLine.front(), forceLine.size(), PrimitiveType::LineStrip);
            window.display();
        }
    }
    return 0;
}

void addCharge(std::vector<CircleShape> &charges, Vector2f pos)
{
    CircleShape circle(POINT_SIZE);
    circle.setPosition(pos);
    circle.setFillColor(mode == CHARGE_POSITIVE ? Color::Red : Color::Blue);
    charges.push_back(circle);
}

void drawForceLine(const std::vector<CircleShape> &charges,
                   std::vector<Vertex> &forceLine1,
                   std::vector<Vertex> &forceLine2,
                   Vector2f startPos)
{

    Vector2f E = {0, 0};
    bool vicinityIsEmpty = true;
    Vector2f curPoint = startPos;

    while (vicinityIsEmpty && inArea(curPoint))
    {
        E = {0, 0};
        for (const CircleShape &charge : charges)
        {
            Vector2f curV = -charge.getPosition() + curPoint;
            float range = len2(curV);

            if (range < POINT_SIZE * POINT_SIZE)
            {
                vicinityIsEmpty = false;
                break;
            }
            float sign = charge.getFillColor() == Color::Blue ? -1 : 1;
            E += normalize(curV) * sign / range;
        }

        if (!vicinityIsEmpty)
            break;
        Vector2f move = normalize(E) * static_cast<float>(POINT_SIZE) / 2.0f;
        forceLine1.push_back(Vertex(curPoint + move, Color::Black));
        curPoint += move;
    }


    curPoint = startPos;
    vicinityIsEmpty = true;
    while (vicinityIsEmpty && inArea(curPoint))
    {
        E = {0, 0};
        for (const CircleShape &charge : charges)
        {
            Vector2f curV = -charge.getPosition() + curPoint;
            float range = len2(curV);

            if (range < POINT_SIZE * POINT_SIZE)
            {
                vicinityIsEmpty = false;
                break;
            }
            float sign = charge.getFillColor() == Color::Blue ? -1 : 1;
            E += normalize(curV) * sign / range;
        }

        if (!vicinityIsEmpty)
            break;
        Vector2f move = (-1.0f) * normalize(E) * static_cast<float>(POINT_SIZE) / 2.0f;
        forceLine2.push_back(Vertex(curPoint + move, Color::Black));
        curPoint += move;
    }
}
