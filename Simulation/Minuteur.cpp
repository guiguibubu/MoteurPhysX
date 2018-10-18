#include "Minuteur.h"

unsigned int Minuteur::getTempsRestant() {
   return timeToReech - dT;
}
bool Minuteur::isFinished() {
   return finished;
}
bool Minuteur::isStarted() {
   return started;
}
bool Minuteur::isConfigured() {
   return timeToReech != 0;
}
void Minuteur::start() {
    oldTime = std::chrono::high_resolution_clock::now();
    finished = false;
    started = true;
}

void Minuteur::refresh() {
   if (!finished && started) {
      dT = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - oldTime).count();
      finished = dT == timeToReech;
      started = !finished;
      timeToReech = (finished) ? 0 : timeToReech;
   }
}

void Minuteur::setDecompte(unsigned int timeToReech) {
   this->timeToReech = timeToReech;
}