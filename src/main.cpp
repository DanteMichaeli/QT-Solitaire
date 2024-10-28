#include <QApplication>
#include <QPushButton>

/**
 * @brief main function
 * @param argc Argument count
 * @param argv Argument vector
 * @return Application exit status
 */
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QPushButton button("Hello, World!");
  button.resize(200, 100);

  button.show();

  return app.exec();
}