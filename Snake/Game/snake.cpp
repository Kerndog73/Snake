//
//  snake.cpp
//  Snake
//
//  Created by Indi Kernick on 3/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "snake.hpp"

#include "rat.hpp"
#include "coin.hpp"
#include "slicer.hpp"
#include "reverser.hpp"
#include "constants.hpp"
#include "invis potion.hpp"
#include "coin consumed.hpp"
#include "render manager.hpp"
#include <Simpleton/Event/manager.hpp>

namespace {
  constexpr Grid::FromVec<Grid::Dir::right, Grid::Dir::down> fromVec {};
  
  //returns a vector pointing from this to prev
  Pos getDirVec(const Pos thisPos, const Pos prevPos) {
    Pos prevToThis = thisPos - prevPos;
    
    //this accounts for the snake going into one side of the window and
    //and coming out from the opposite side of the window
    if (prevToThis.x > 1) {
      prevToThis.x -= GAME_SIZE.x;
    }
    if (prevToThis.y > 1) {
      prevToThis.y -= GAME_SIZE.y;
    }
    if (prevToThis.x < -1) {
      prevToThis.x += GAME_SIZE.x;
    }
    if (prevToThis.y < -1) {
      prevToThis.y += GAME_SIZE.y;
    }
    
    return prevToThis;
  }
}

Snake::Snake(const Pos center)
  : positions({
      {center.x - 1, center.y},
      center,
      {center.x + 1, center.y},
      {center.x + 2, center.y}
    }) {}

void Snake::move(const Grid::Dir newDir) {
  assert(positions.size());
  if (Grid::opposite(newDir) != currentDir) {
    nextDir = newDir;
  }
}

bool Snake::isEating(const Rat &rat) const {
  assert(positions.size());
  return state == State::EATING && positions.front() == rat.getPos();
}

bool Snake::colliding(const Pos pos, const bool withHead) const {
  return std::any_of(
    positions.cbegin() + !withHead,
    positions.cend(),
    [pos] (const Pos thisPos) {
      return pos == thisPos;
    }
  );
}

bool Snake::isDead() const {
  return state == State::DEAD;
}

bool Snake::tryToConsume(Item &item) {
  if (head() == item.getPos() && item.isAlive()) {
    item.consume();
    Item *const ptr = &item;
    
    if (dynamic_cast<Reverser *>(ptr)) {
      std::reverse(positions.begin(), --positions.end());
      currentDir = fromVec(getDirVec(positions[0], positions[1]));
      nextDir = currentDir;
    } else if (dynamic_cast<Rat *>(ptr)) {
      state = State::EATING;
    } else if (dynamic_cast<Slicer *>(ptr)) {
      positions.resize(std::max(size_t(4), positions.size() / 2));
    } else if (dynamic_cast<Coin *>(ptr)) {
      Game::evtMan->emit<CoinConsumed>();
    } else if (dynamic_cast<InvisPotion *>(ptr)) {
      timeTillVisible = INVIS_DURATION;
    }
    
    return true;
  }
  return false;
}

Pos Snake::head() const {
  return positions.front();
}

Pos Snake::tail() const {
  return positions.back();
}

void Snake::update() {
  if (state == State::EATING) {
    state = State::ALIVE;
  } else {
    positions.pop_back();
  }
  positions.push_front((positions.front() + toVec(nextDir) + GAME_SIZE) % GAME_SIZE);
  currentDir = nextDir;
  if (colliding(head(), false) && head() != tail()) {
    state = State::DEAD;
  }
  if (timeTillVisible) {
    timeTillVisible--;
  }
}

namespace {
  double getBodySpriteAngle(const Grid::Dir prevToThisDir) {
    return Grid::toNum<double>(prevToThisDir, 90.0);
  }

  std::string getBodySpriteTurnName(const Grid::Dir prevToThisDir, const Grid::Dir thisToNextDir) {
    const Grid::SignedDirType diff = Grid::diff(prevToThisDir, thisToNextDir);
    if (diff == 0) {
      return "straight";
    } else if (diff == 1) {
      return "right corner";
    } else if (diff == -1) {
      return "left corner";
    } else {
      assert(false);
      return "";
    }
  }

  void renderBody(
    RenderManager &renderer,
    const std::string &name,
    const Pos pos,
    const Pos prevToThisVec,
    const Pos thisToNextVec,
    const bool visible
  ) {
    constexpr Grid::FromVec<Grid::Dir::right, Grid::Dir::down> fromVec {};
    
    const Grid::Dir prevToThisDir = fromVec(prevToThisVec);
    const Grid::Dir thisToNextDir = fromVec(thisToNextVec);
    renderer.renderTile(
      (visible ? "" : "invis ") + name + ' ' + getBodySpriteTurnName(prevToThisDir, thisToNextDir),
      pos,
      getBodySpriteAngle(prevToThisDir)
    );
  }

  std::string dirToString(const Grid::Dir dir) {
    static const std::string NAMES[4] = {
      "up", "right", "down", "left"
    };
    return NAMES[Grid::toNum<size_t>(dir)];
  }
}

void Snake::render(RenderManager &renderer) const {
  if (positions.size() == 0) {
    return;
  }
  
  const bool visible = timeTillVisible == 0;
  const bool eating = state == State::EATING;

  if (eating) {
    renderer.renderTile("eat rat " + dirToString(currentDir), head());
  } else {
    renderer.renderTile("head", head(), getBodySpriteAngle(currentDir));
  }

  Pos nextPos = positions[2];
  Pos thisToNextVec = getDirVec(positions[1], nextPos);
  renderBody(renderer, "head", positions[1], thisToNextVec, getDirVec(positions[0], positions[1]), visible);
  
  for (auto b = positions.cbegin() + 2; b != positions.cend() - 2; ++b) {
    const Pos pos = nextPos;
    nextPos = *(b + 1);
    const Pos prevToThisVec = getDirVec(pos, nextPos);
    if (visible) {
      renderBody(renderer, "body", pos, prevToThisVec, thisToNextVec, visible);
    }
    thisToNextVec = prevToThisVec;
  }
  
  const Pos prevToThisVec = getDirVec(nextPos, positions.back());
  
  if (visible || (!visible && !eating)) {
    renderBody(renderer, eating ? "body" : "tail", nextPos, prevToThisVec, thisToNextVec, visible);
  }
  thisToNextVec = prevToThisVec;

  const double tailAngle = getBodySpriteAngle(fromVec(thisToNextVec));
  renderer.renderTile(eating ? "tail grow" : "tail", tail(), tailAngle);
}
