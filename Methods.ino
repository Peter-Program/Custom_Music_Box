/*
 * Checks the position of the chests lid
 * LOW is open so set to true, 
 * HIGH is closed so set to false
 * Returns true if chest is open and its previous state was closed
 * Returns false if chest is closed
 */
boolean chestOpenOrClosed(int button) {

  boolean chestOpen = false;
  // Checking if the chest has just been lifted
  // ChestStateClosed exists for not continuesly playing files if the lid is open
  if (digitalRead(button) == LOW && chestStateClosed == true) {
      chestOpen = true;
      chestStateClosed = false;
  }

  return chestOpen;

}