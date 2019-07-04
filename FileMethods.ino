/*
 * Use this so that multiple audio files are not being played at the same time
 * Also checks for if the lid on the chest gets closed when a file is being 
 * played and could stop playing the file
 */
void waitForSongToFinish() {
  while(file.isPlaying() == 1){
      // Stay in loop while a file is playing
      Serial.println("Playing File");
      if (digitalRead(chestButton) == HIGH && rndFilePlaying) {         // Chest gets closed while playing so stop playing file
          chestStateClosed = true;                                      // Setting state of chest lid to closed
          file.stopPlayback();                                          // Stops the audio file
          file.disable();
          Serial.println("Stop File");
          rndFilePlaying = false;                                       // Remembers that a file is not currently being played    
      }
      delay(300);
  }
}

/*
 * Plays the audio file by the name of the passed in perameter
 */
void playFile(String name) {

  //Need to convert String to char array for file.play to function correctly
  char nameCharArr[9];
  name.toCharArray(nameCharArr, 9);

  // Special case of Sun's Song
  if(name.compareTo("E114.wav") == 0){                                 // sun Song is played
    Serial.println("In Special case Sun Song");
      file.play(nameCharArr);
    waitForSongToFinish();
    file.play("E108.wav");                                      // Rooster
  } else {

    Serial.println("About to play file");
    file.play(nameCharArr);
  }
  waitForSongToFinish();
}

/*
 * Plays a random audio file from the zeldaSongs, easterEggs, or 
 * soundEffects lists
 */
void playRandomFile() {
  int rnd = random(1, SOUNDFILESIZE + 1);
  String fileName = "E";
  String rndStr = String(rnd);

  if(rnd < 10){
    fileName.concat("0");
  }

  if(rnd < 100){
    fileName.concat("0");
  }

  fileName.concat(rndStr);
  fileName.concat(".wav");

  Serial.println(fileName);
  
  rndFilePlaying = true;
  playFile(fileName);
  //waitForSongToFinish();

}