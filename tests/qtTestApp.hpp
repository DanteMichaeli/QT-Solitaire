#ifndef QT_TEST_APP_HPP
#define QT_TEST_APP_HPP

#include <QGuiApplication>

/**
 * @brief Fixture to set up a QGuiApplication for all tests.
 */
struct QtTestApp {
  static QGuiApplication* app;

  QtTestApp() {
    if (!app) {
      static int argc = 1;
      static char* argv[] = {const_cast<char*>("test_app")};
      app = new QGuiApplication(argc, argv);
    }
  }
};

#endif  // QT_TEST_APP_HPP
