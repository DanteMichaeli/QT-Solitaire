#ifndef KLONDIKE_LAYOUT_HPP
#define KLONDIKE_LAYOUT_HPP

#include "layout.hpp"

class KlondikeLayout : public Layout {
 public:
  KlondikeLayout(QGraphicsScene* scene, Game* game);

  static constexpr double horFactor = 0.1;
  static constexpr double vertFactor = 0.05;
  static constexpr double leftMargin = 1;
  static constexpr double topMargin = 0.3;
  static constexpr double minLMargin = 0.0;
  static constexpr double maxScale = 1.2;
  static constexpr double minScale = 0.25;
  static constexpr int horizontalPiles = 7;
  static constexpr int verticalPiles = 2;

  void init() override;

  void resize(QSizeF& newSize) override;
};

#endif