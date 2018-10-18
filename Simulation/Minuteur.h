#include <chrono>

#ifndef MINUTEUR_H
#define MINUTEUR_H

class Minuteur {

private:
   std::chrono::time_point<std::chrono::high_resolution_clock> oldTime;
   unsigned int timeToReech;
   unsigned short dT;
   bool finished;
   bool started;

public:
   Minuteur() : Minuteur(unsigned int{}) {}
   Minuteur(unsigned int timeToReech) : timeToReech{ timeToReech }, dT{}, finished{}, oldTime{}, started{} {}
   ~Minuteur() = default;

   unsigned int getTempsRestant();
   void setDecompte(unsigned int timeToReech);
   bool isFinished();
   bool isStarted();
   bool isConfigured();
   void start();
   void refresh();
};

#endif //MINUTEUR_H
