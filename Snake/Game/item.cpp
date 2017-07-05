//
//  item.cpp
//  Snake
//
//  Created by Indi Kernick on 4/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "item.hpp"

Item::Item(const Pos pos, const unsigned lifeTime)
  : pos(pos), timeTillDeath(lifeTime) {}

Pos Item::getPos() const {
  return pos;
}

bool Item::isAlive() const {
  return state == State::ALIVE;
}

void Item::consume() {
  assert(isAlive());
  state = State::BEING_CONSUMED;
}

void Item::update() {
  if (state == State::ALIVE) {
    timeTillDeath--;
    if (timeTillDeath == 0) {
      state = State::DESPAWNING;
    }
  }
  if (state == State::SPAWNING) {
    state = State::ALIVE;
  }
}

Item::State Item::getState() const {
  return state;
}
