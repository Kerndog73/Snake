//
//  snake.hpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_snake_hpp
#define game_snake_hpp

#include <deque>
#include "types.hpp"

class Rat;
class Item;
class RenderManager;

class Snake {
public:
  explicit Snake(Pos);
  
  void move(Math::Dir);
  bool isEating(const Rat &) const;
  bool colliding(Pos, bool = true) const;
  bool isDead() const;
  
  bool tryToConsume(Item &);
  
  Pos head() const;
  Pos tail() const;
  
  void update();
  void render(RenderManager &) const;
  
private:
  std::deque<Pos> positions;
  Math::Dir currentDir = Math::Dir::LEFT;
  Math::Dir nextDir = Math::Dir::LEFT;
  
  enum class State : uint8_t {
    ALIVE,
    EATING,
    DEAD
  } state = State::ALIVE;
};

#endif
