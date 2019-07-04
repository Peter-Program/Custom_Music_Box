/* 
 * Creates a random number that then multiplies naviInterval
 * to create a random occurrence of navi sound effect.
 * The interval has a range of 1 minute to 60 minutes 
 */
void naviMultInterval() {
  int mult = random(1, 61);
  naviInterval = baseNaviInterval * mult;

  Serial.println(naviInterval);
  int x = naviInterval / 60000;
  Serial.print("Next Navi Interval ");
  Serial.println(x);
}

/* 
 * Plays a navi sound effect in relation to the passed in random number n
 */
void naviEffects(int num) {
  // basic navi
  if (num < naviSoundsSize) {
    playFile(naviSounds[num]);
  }
  // Navi - Hey Listen combo sound
  else if ( num == naviSoundsSize) {
    playFile("N002.wav");
    waitForSongToFinish();
    playFile("N003.wav");
  }
  // Poe
  else if ((num == naviSoundsSize + 1) && !timeDay) {
    int gap = random(2, 21);                // Creates random time delay for when a Poe laughs after appearing
    int gap2 = random(3, 13);               // Creates random time delay for when The Poe vanishes
    playFile("N027.wav");                   //PoeAppear
    waitForSongToFinish();
    delay(gap * 1000);
    playFile("N028.wav");                   //PoeLaugh
    waitForSongToFinish();
    delay(gap2 * 1000);
    playFile("N029.wav");                   //PoeVanish
  }
  // Navi in/out
  else {
    if (naviOut) {
      playFile("N008.wav");                 //NaviIn
      naviOut = false;
    }
    else {
      playFile("N009.wav");                 //NaviOut
      naviOut = true;
    }
  }
}

/*
 * Plays a navi sound effect if enough time has passed for the 
 * naviInterval to be lessthen the amount of time that has passee
 */
void playNaviSound() {
    if (currTime - prevNavi > naviInterval) {
      Serial.println("Playing Navi File");
      naviEffects(random(0, 10));                   // Call random navi sound
      prevNavi = currTime;                          // assign new time value
      naviMultInterval();                           // assign new interval for navi sound effects
      waitForSongToFinish();
    }
}