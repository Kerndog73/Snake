//
//  rat.cpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "rat.hpp"

#include "render manager.hpp"

Rat::Rat(const Pos pos)
  : pos(pos) {}

void Rat::reset(const Pos newPos) {
  pos = newPos;
  beingEaten = false;
}

Pos Rat::getPos() const {
  return pos;
}

void Rat::eat() {
  beingEaten = true;
}

void Rat::render(RenderManager &renderer) const {
  if (not beingEaten) {
    renderer.renderTile("rat", pos);
  }
}
