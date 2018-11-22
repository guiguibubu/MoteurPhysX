#include "commonImport.h"
#include "MyCallback.h"
#include "Simulation.h"

#include <string>

class Simulation;

void MyCallback::onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count) {
   std::cout << "onContactModify" << std::endl;
   for (physx::PxU32 i = 0; i < count; i++)
   {
      auto pair = pairs[i];

      if (pairContient(pair, Simulation::FilterGroup::eGOAL))
      {
         auto IdOtherObject = getFlag(getOtherFilterData(pair, Simulation::FilterGroup::eGOAL));

         switch (IdOtherObject) {
         case Simulation::FilterGroup::eCARGO: Simulation::get().goalAtteint();  break;
         default: ignoreContact(pair); break;
         }

         break;
      }
      else if (pairContient(pair, Simulation::FilterGroup::eVEHICULE)) {

         auto filterData = getOtherFilterData(pair, Simulation::FilterGroup::eVEHICULE);
         auto IdOtherObject = getFlag(filterData);

         switch (IdOtherObject) {
         case Simulation::FilterGroup::eBALLE: pairs->contacts.ignore(0);  break;
         default: ignoreContact(pair); break;
         }

         break;
      }
      else if (pairContient(pair, Simulation::FilterGroup::eBALLE)) {

         auto filterData = getOtherFilterData(pair, Simulation::FilterGroup::eBALLE);
         auto IdOtherObject = getFlag(filterData);

         switch (IdOtherObject) {
         case Simulation::FilterGroup::eBALLE: ignoreContact(pair); break;
         case Simulation::FilterGroup::eSOL: break;
         case Simulation::FilterGroup::eCARGO: break;
         default: ignoreContact(pair); break;
         }

         break;
      }
   }
}

void MyCallback::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {
   std::cout << "onContact" << std::endl;
}

bool MyCallback::pairContient(const physx::PxContactModifyPair& pair, const physx::PxU32& flag) {
   return (getFlag(pair, 0) == flag) || (getFlag(pair, 1) == flag);
}

physx::PxU32 MyCallback::getFlag(const physx::PxContactModifyPair& pair, const short& index) {
   return getFlag(getFilterData(pair, index));
}

physx::PxU32 MyCallback::getFlag(const physx::PxFilterData& filterData) {
   return filterData.word0;
}

physx::PxFilterData MyCallback::getFilterData(const physx::PxContactModifyPair& pair, const short& index) {
   return pair.shape[index]->getSimulationFilterData();
}

physx::PxFilterData MyCallback::getOtherFilterData(const physx::PxContactModifyPair& pair, const physx::PxU32& flag) {
   physx::PxFilterData otherFilterData = physx::PxFilterData();
   if (pairContient(pair, flag)) {
      otherFilterData = (getFlag(pair, 0) == flag) ? getFilterData(pair, 1) : getFilterData(pair, 0);
   }
   return otherFilterData;
}

void MyCallback::ignoreContact(physx::PxContactModifyPair& pair) {
   for (int i = 0; i < pair.contacts.size(); i++) {
      pair.contacts.ignore(i);
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