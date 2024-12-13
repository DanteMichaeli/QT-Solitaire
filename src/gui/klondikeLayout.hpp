#ifndef KLONDIKE_LAYOUT_HPP
#define KLONDIKE_LAYOUT_HPP

#include "layout.hpp"

/**
 * @class KlondikeLayout
 * @brief A specific layout implementation for the Klondike Solitaire game.
 *
 * The KlondikeLayout class extends the Layout class and provides specific
 * implementations for initializing and resizing the visual components of the
 * Klondike Solitaire game, such as card piles, foundation stacks, and the
 * tableau. This layout is tailored to the Klondike variation of Solitaire.
 */
class KlondikeLayout : public Layout {
 public:
  /**
   * @brief Constructs a KlondikeLayout object.
   *
   * Initializes the layout for Klondike Solitaire by setting the scene and
   * game.
   *
   * @param scene The QGraphicsScene where the graphical items will be placed.
   * @param game The Game object that handles the logic of the Klondike game.
   */
  KlondikeLayout(QGraphicsScene* scene, Game* game);

  /// Factor that determines the spacing of piles in Y direction. Based on Pile
  /// height.
  static constexpr double horFactor = 0.1;

  /// Factor that determines the spacing of piles in X direction. Based on Pile
  /// width.
  static constexpr double vertFactor = 0.05;

  /// Factor for the max left margin. Based on Pile width.
  static constexpr double leftMargin = 1;

  /// Factor for the mtop margin. Based on Pile height.
  static constexpr double topMargin = 0.3;

  /// Minimum factor allowed for the left margin. Based on Pile width.
  static constexpr double minLMargin = 0.0;

  /// Maximum scaling factor allowed for the elements in the scene
  static constexpr double maxScale = 1.2;

  /// Minimum scaling factor allowed for the elements in the scene
  static constexpr double minScale = 0.25;

  /// Number of columns in this Klondike game's layout
  static constexpr int horizontalPiles = 7;

  /// Number of rows in this Klondike game's layout
  static constexpr int verticalPiles = 2;

  /**
   * @brief Initializes the layout by setting up the graphical components
   * specific to Klondike.
   *
   * This function is called to initialize all the graphical items (such as
   * piles, cards, etc.) according to the Klondike game rules and layout.
   */
  void init() override;

  /**
   * @brief Resizes the layout according to the new window size.
   *
   * This function adjusts the positions and sizes of the graphical elements
   * based on the new size of the scene.
   *
   * @param newSize The new size to which the layout should be adjusted.
   */
  void resize(const QSizeF& newSize) override;
};

#endif  // KLONDIKE_LAYOUT_HPP
