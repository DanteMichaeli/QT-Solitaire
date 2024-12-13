#include "klondikeLayout.hpp"

KlondikeLayout::KlondikeLayout(QGraphicsScene* scene, Game* game)
    : Layout(scene, game) {
  init();
}

void KlondikeLayout::init() {
  auto game = this->getGame();
  auto scene = this->getScene();

  // Add deck and Waste pile
  scene->addItem(game->getDeck());
  scene->addItem(game->getWPile());

  // Add all seven Klondike piles to the scene
  for (auto& ptr : game->getKPiles()) {
    scene->addItem(ptr);
  }
  // Add all four Target piles to the scene
  for (auto& ptr : game->getTPiles()) {
    scene->addItem(ptr);
  }
}

void KlondikeLayout::resize(const QSizeF& newSize) {
  const double width = newSize.width() - 30;
  const double height = newSize.height() - 30;

  auto scene = this->getScene();
  auto game = this->getGame();
  auto deck = game->getDeck();
  auto wPile = game->getWPile();

  const int pileRealWidth = deck->getWidth();
  const int pileRealHeight = deck->getHeight();

  // Initial margins in pixels
  double topMpix = pileRealHeight * topMargin;
  double leftMpix = pileRealWidth * leftMargin;

  // Dynamically adjust margin and scale
  const double availableW = width - 2 * leftMpix;
  const double totSpacingW = (horizontalPiles + 1) * pileRealWidth * horFactor;
  const double totPileW = horizontalPiles * pileRealWidth;
  const double initialScale = availableW / (totSpacingW + totPileW);

  double scale;    // Final scale
  double xDiff;    // Spacing in x dir
  double yDiff;    // Spacing in y dir
  double pWidth;   // Scaled width of pile
  double pHeight;  // Scaled Hieght of pile

  // Check if over threshold
  if (initialScale > maxScale) {
    // Calculate the space that the piles took
    // Calculate the necessary margins to left and right side
    scale = maxScale;
    pWidth = pileRealWidth * scale;
    pHeight = pileRealHeight * scale;
    xDiff = pWidth * horFactor;
    yDiff = pHeight * vertFactor;
    const double spacing = xDiff * (horizontalPiles + 1);
    const double space = pWidth * horizontalPiles;
    const double takenSpace = spacing + space;
    leftMpix = (width - takenSpace) / 2.0;
  } else {
    // Calculate if max taken space is over screen width
    const double spacing = pileRealWidth * horFactor * (horizontalPiles + 1);
    const double maxSpacing = spacing * maxScale;
    const double space = pileRealWidth * horizontalPiles;
    const double maxSpace = space * maxScale;
    const double maxTakenSpace = maxSpacing + maxSpace;
    const double spaceLeft = width - maxTakenSpace;
    // If not, scale is max scale and calculate margin lengths.
    // Otherwise margin is zero and calculate new scale.
    if (spaceLeft >= 2 * minLMargin * pileRealWidth) {
      scale = maxScale;
      leftMpix = spaceLeft / 2.0;
    } else {
      leftMpix = minLMargin * pileRealWidth;
      const double spacing = pileRealWidth * horFactor * (horizontalPiles + 1);
      const double space = pileRealWidth * horizontalPiles;
      scale = std::max((width - 2 * leftMpix) / (spacing + space), minScale);
    }
    // Set values
    pWidth = pileRealWidth * scale;
    pHeight = pileRealHeight * scale;
    xDiff = pWidth * horFactor;
    yDiff = pHeight * vertFactor;
  }

  // Begin scaling and position changes.
  deck->setScale(scale);
  wPile->setScale(scale);
  deck->setPos(leftMpix + xDiff, topMpix + yDiff);
  wPile->setPos(leftMpix + 2 * xDiff + pWidth, topMpix + yDiff);
  deck->setCardsPrevScenePos();
  wPile->setCardsPrevScenePos();

  double xOffset = leftMpix + xDiff;
  double yOffset = topMpix + 2 * yDiff + pHeight;
  for (auto& kp : game->getKPiles()) {
    kp->setPos(xOffset, yOffset);
    kp->setScale(scale);
    kp->setCardsPrevScenePos();
    xOffset += xDiff + pWidth;
  }

  xOffset = leftMpix + 4 * xDiff + 3 * pWidth;
  yOffset = topMpix + yDiff;
  for (auto& tp : game->getTPiles()) {
    tp->setPos(xOffset, yOffset);
    tp->setScale(scale);
    tp->setCardsPrevScenePos();
    xOffset += xDiff + pWidth;
  }
}
