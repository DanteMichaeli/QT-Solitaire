#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScreen>

#include "game.hpp"

class Layout {
 public:
  explicit Layout(QGraphicsScene* scene, Game* game)
      : scene_(scene), game_(game) {}

  QGraphicsScene* getScene() { return scene_; }

  Game* getGame() { return game_; }

  virtual ~Layout() = default;

  virtual void init() = 0;

  virtual void resize(QSizeF& newSize) = 0;

 private:
  QGraphicsScene* scene_;
  Game* game_;
};

#endif