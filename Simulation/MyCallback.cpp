#include "commonImport.h"
#include "MyCallback.h"
#include "Simulation.h"

#include <string>

class Simulation;

void MyCallback::onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count) {
   std::cout << "onContactModify" << std::endl;
   for (physx::PxU32 i = 0; i < count; i++)
   {
      auto shape0Id = pairs->shape[0]->getSimulationFilterData().word0;
      auto shape1Id = pairs->shape[1]->getSimulationFilterData().word0;

      if ((shape0Id == Simulation::getWallFilterGroup()) || (shape1Id == Simulation::getWallFilterGroup()))
      {
         auto filterData = (shape0Id == Simulation::getWallFilterGroup()) ? pairs->shape[1]->getSimulationFilterData() : pairs->shape[0]->getSimulationFilterData();
         auto IdOtherObject = filterData.word0;

         short index = 0;
         bool found = false;
         while (index < Simulation::get().getNbBallMax() && !found) {
            found = Simulation::getBallFilterGroup(++index) == IdOtherObject;
            i++;
         }
         Simulation::get().contactDetected(index);

         switch (index) {
         case 1: break;
         case 2: pairs->contacts.ignore(0); break;
         case 3: break;
         case 4: pairs->contacts.ignore(0); break;
         default: break;
         }

         break;
      }
   }
}

void MyCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {
   std::cout << "onContact" << std::endl;
   for (physx::PxU32 i = 0; i < nbPairs; i++)
   {
      const physx::PxContactPair& cp = pairs[i];

      if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
      {
         if ((pairHeader.actors[0] == Simulation::get().getWall()) || (pairHeader.actors[1] == Simulation::get().getWall()))
         {
            auto filterData = (Simulation::get().getWall() == pairHeader.actors[0]) ? pairs->shapes[1]->getSimulationFilterData() : pairs->shapes[0]->getSimulationFilterData();
            auto IdOtherObject = filterData.word0;

            short index = 0;
            bool found = false;
            while (index < Simulation::get().getNbBallMax() && !found) {
               found = Simulation::getBallFilterGroup(++index) == IdOtherObject;
               i++;
            }
            Simulation::get().contactDetected(index);
            break;
         }
      }
   }
}

void MyCallback::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {
   std::cout << "onConstraintBreak" << std::endl;
}
void MyCallback::onWake(physx::PxActor** actors, physx::PxU32 count) {
   std::cout << "onWake" << std::endl;
}
void MyCallback::onSleep(physx::PxActor** actors, physx::PxU32 count) {
   std::cout << "onSleep" << std::endl;
}
void MyCallback::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {
   std::cout << "onTrigger" << std::endl;
}
void MyCallback::onAdvance(const physx::PxRigidBody*const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) {
   std::cout << "onAdvance" << std::endl;
}