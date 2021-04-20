#include <Clock.h>

Clock clock(true);

void setup() {
  clock.start(80);
}

void loop() {
  clock.set(clock.getTime());
  clock.display();
}
