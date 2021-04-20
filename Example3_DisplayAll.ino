#include <Clock.h>

Clock clock(true);

void setup() {
  clock.start(80);
}

void loop() {
  if (Button2 == HIGH){
    clock.set(clock.DHT11Temperature(true));
    clock.display();
    while (Button2 == HIGH);
    delay(100);
  }
  else if (Button3 == HIGH){
    clock.set(clock.DHT11Humidity());
    clock.display();
    while (Button3 == HIGH);
    delay(100);
  }
  else if (Button4 == HIGH){
    clock.set(clock.getDate());
    clock.display();
    while (Button4 == HIGH);
    delay(100);
  }
  while (Button2 != HIGH && Button3 != HIGH && Button4 != HIGH){
    clock.set(clock.getTime());
    clock.display();
  }
  delay(500);
}
