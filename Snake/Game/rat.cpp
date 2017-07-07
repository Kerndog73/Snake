//
//  rat.cpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "rat.hpp"

#include "render manager.hpp"

Rat::Rat()
  : Item({-1, -1}) {}

Rat::Rat(const Pos pos)
  : Item(pos) {}

void Rat::render(RenderManager &renderer) const {
  if (pos != Pos(-1, -1)) {
    const State state = getState();
    if (state == State::ALIVE) {
      renderer.renderTile("rat", pos);
    } else if (state == State::SPAWNING) {
      renderer.renderTile("rat spawn", pos);
    }
  }
}
