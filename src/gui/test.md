

void GameView::initializeGame() {
  // Clear the scene before adding a new card
  scene->clear();

  // Create a single Card object manually (without using the Deck class)
  testcard = std::make_unique<Card>(Suit::HEARTS, Rank::ACE);
  testcard->flipUp();

  // Create a CardItem using the raw pointer to the manually created Card
  CardItem* cardItem = new CardItem(testcard.get());
  scene->addItem(cardItem);

  // Set the position and scale of the CardItem
  cardItem->setScale(0.3);
  cardItem->setPos(100, 100);
}