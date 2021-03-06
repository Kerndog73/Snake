//
//  app impl.hpp
//  Snake
//
//  Created by Indi Kernick on 2/7/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef game_app_impl_hpp
#define game_app_impl_hpp

#include <list>
#include <vector>
#include "rat.hpp"
#include "snake.hpp"
#include "score.hpp"
#include "item.hpp"
#include "render manager.hpp"
#include <Simpleton/Application/sdl app.hpp>

using ItemFactory = std::unique_ptr<Item> (*)(Pos);

class AppImpl final : public Game::SDLApp<std::chrono::duration<uint64_t, std::milli>> {
public:
  AppImpl() = default;
  
private:
  RenderManager renderMan;
  uint64_t animProg = 0;
  Snake snake;
  Rat rat;
  Score score;
  std::list<std::unique_ptr<Item>> items;
  std::vector<ItemFactory> itemFactories;
  std::vector<double> itemProbs;
  
  enum class State {
    HOME,
    GAME,
    PAUSE,
    DEAD
  } state = State::HOME;

  bool init() override;
  void quit() override;
  bool input(uint64_t) override;
  bool update(uint64_t) override;
  void render(uint64_t) override;
  
  void updateGame();
  
  void spawnItemIfShould();
  Pos getFreePos() const;
  void snakeInput(SDL_Scancode);
  void changeState();
  void reset();
};

#endif
