#include <Clock.h>

Clock clock(true);

void setup() {
  clock.start(80);
}

void loop() {
  clock.alarmSet(1, Button1, Button2, Button3, Button4);
  clock.alarmArm(1, true, Button1, true, true, 255,255,0, 2,1,4,1,2,1,4,1);
  clock.set(clock.getTime());
  clock.display();
}
