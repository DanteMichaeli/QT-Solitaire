#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <QGraphicsScene>
#include <QGraphicsView>

#include "game.hpp"

/**
 * @class Layout
 * @brief Abstract base class for managing the layout of graphical elements
 *        in the game view.
 *
 * The Layout class provides an interface for setting up and resizing the visual
 * components of the game, such as the deck and cards, on a given
 * QGraphicsScene. It is meant to be inherited by specific layout types for
 * different game setups.
 */
class Layout {
 public:
  /**
   * @brief Constructs a Layout object.
   *
   * Initializes the scene and game references used for managing the graphical
   * layout.
   *
   * @param scene The QGraphicsScene where graphical items will be placed.
   * @param game The Game object that contains the game logic.
   */
  explicit Layout(QGraphicsScene* scene, Game* game)
      : scene_(scene), game_(game) {}

  /**
   * @brief Gets the QGraphicsScene associated with this layout.
   *
   * @return The QGraphicsScene used for displaying the game items.
   */
  QGraphicsScene* getScene() const { return scene_; }

  /**
   * @brief Gets the Game object associated with this layout.
   *
   * @return The Game object that handles the game logic.
   */
  Game* getGame() const { return game_; }

  /**
   * @brief Virtual destructor for the Layout class.
   *
   * The destructor is virtual to allow derived classes to clean up resources.
   */
  virtual ~Layout() = default;

  /**
   * @brief Initializes the layout by setting up the graphical components.
   *
   * This method is pure virtual, requiring implementation in derived classes.
   */
  virtual void init() = 0;

  /**
   * @brief Resizes the layout according to the new window size.
   *
   * This method is pure virtual, requiring implementation in derived classes.
   *
   * @param newSize The new size to which the layout should be adjusted.
   */
  virtual void resize(const QSizeF& newSize) = 0;

 private:
  QGraphicsScene* scene_;  ///< The QGraphicsScene for placing game items
  Game* game_;             ///< The Game object containing game logic
};

#endif  // LAYOUT_HPP
