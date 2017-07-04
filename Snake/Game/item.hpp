//
//  item.hpp
//  Snake
//
//  Created by Indi Kernick on 4/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_item_hpp
#define game_item_hpp

#include "types.hpp"

class RenderManager;

class Item {
public:
  explicit Item(Pos);
  virtual ~Item() = default;
  
  Pos getPos() const;
  bool isAlive() const;
  virtual void consume();
  
  virtual void update();
  virtual void render(RenderManager &) const = 0;

protected:
  Pos pos;

  enum class State : uint8_t {
    SPAWNING,
    ALIVE,
    BEING_CONSUMED
  };
  
  State getState() const;

private:
  State state = State::SPAWNING;
};

#endif