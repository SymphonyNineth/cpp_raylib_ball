#include "raylib.h"
#include <iostream>
#include <functional>

class GeometricalObject
{
public:
  int x, y;
  Color color;

  GeometricalObject(int x, int y, Color color) : x(x), y(y), color(color)
  {
  }

  Vector2 GetCenter() const
  {
    return {x, y};
  }

  void GoTo(int newX, int newY)
  {
    x = newX;
    y = newY;
  }

  void MoveX(int newX = 10)
  {
    GoTo(x + newX, y);
  }

  void MoveY(int newY = 10)
  {
    GoTo(x, y + newY);
  }

  void MoveRight(int newX = 10)
  {
    MoveX(newX);
  }

  void MoveLeft(int newX = 10)
  {
    MoveX(-newX);
  }

  void MoveUp(int newY = 10)
  {
    MoveY(-newY);
  }

  void MoveDown(int newY = 10)
  {
    MoveY(newY);
  }
};

class CircleObject : public GeometricalObject
{
public:
  int radius;

  CircleObject(int x, int y, int radius, Color color) : GeometricalObject(x, y, color), radius(radius)
  {
  }

  float GetRadius() const
  {
    return radius;
  }
};

class RectangleObject : public GeometricalObject
{
public:
  int width, height, direction = 15;

  RectangleObject(int x, int y, int width, int height, Color color) : GeometricalObject(x, y, color), width(width), height(height)
  {
  }

  Rectangle GetRect() const
  {
    return {x, y, width, height};
  }

  void Fall()
  {
    y += direction;
  }

  void setDirection(int newDirection)
  {
    direction = newDirection;
  }

  void Reset(int windowWidth, int windowHeight)
  {
    x = rand() % (windowWidth - width) + width;
    y = height;
  }

  void Reverse(int windowWidth)
  {
    x = rand() % (windowWidth - width) + width;
    direction = -direction;
  }
};

void handleCircleMovement(CircleObject *circle, int windowWidth, int windowHeight)
{

  if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && circle->x + circle->radius < windowWidth)
  {
    circle->MoveRight();
  }

  if ((IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) && circle->x > circle->radius)
  {
    circle->MoveLeft();
  }

  if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) && circle->y > circle->radius)
  {
    circle->MoveUp();
  }

  if ((IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) && circle->y + circle->radius < windowHeight)
  {
    circle->MoveDown();
  }
}

void handleAxeMovement(RectangleObject *axe, int windowWidth, int windowHeight)
{
  axe->Fall();

  if (axe->y + axe->width >= windowHeight || axe->y <= 0)
  {
    axe->Reverse(windowWidth);
  }
}

void renderButton(Rectangle buttonRect, Color buttonColor, const char *buttonText, const std::function<void()> &clickHandler)
{
  DrawRectangle(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, buttonColor);

  Vector2 textPosition = {buttonRect.x + buttonRect.width / 2 - MeasureText(buttonText, 20) / 2, buttonRect.y + buttonRect.height / 2 - 10};
  DrawText(buttonText, textPosition.x, textPosition.y, 20, WHITE);

  if (CheckCollisionPointRec(GetMousePosition(), buttonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
  {
    clickHandler();
  }
}

void resetGame(bool *gameOver, RectangleObject *axe, CircleObject *player, int windowWidth, int windowHeight)
{
  axe->Reset(windowWidth, windowHeight);
  player->GoTo(windowWidth / 2, windowHeight / 2);
  *gameOver = false;
}

void showGameOverScreen(bool *gameOver, RectangleObject *axe, CircleObject *player, int windowWidth, int windowHeight)
{
  RectangleObject resetButton(windowWidth / 2 - 60, windowHeight / 2 - 30, 120, 60, GREEN);
  DrawText("Game over", windowWidth / 2 - 50, windowHeight / 2 - 100, 20, RED);
  renderButton(resetButton.GetRect(), resetButton.color, "Play Again", [gameOver, axe, player, windowWidth, windowHeight]()
               { resetGame(gameOver, axe, player, windowWidth, windowHeight); });
}

int main()
{
  constexpr short windowWidth = 800;
  constexpr short windowHeight = 450;
  InitWindow(windowWidth, windowHeight, "The Axe");

  CircleObject circle(windowWidth / 2, windowHeight / 2, 10, GREEN);
  RectangleObject axe(300, 0, 60, 90, RED);
  SetTargetFPS(60);
  bool gameOver = false;

  while (!WindowShouldClose())
  {
    gameOver = CheckCollisionCircleRec(circle.GetCenter(), circle.GetRadius(), axe.GetRect());
    BeginDrawing();
    ClearBackground(WHITE);

    if (gameOver)
    {
      showGameOverScreen(&gameOver, &axe, &circle, windowWidth, windowHeight);
    }
    else
    {

      DrawCircle(circle.x, circle.y, circle.radius, circle.color);
      DrawRectangle(axe.x, axe.y, axe.width, axe.height, axe.color);

      handleCircleMovement(&circle, windowWidth, windowHeight);
      handleAxeMovement(&axe, windowWidth, windowHeight);
    }

    EndDrawing();
  }

  std::cout << "Done" << std::endl;
}