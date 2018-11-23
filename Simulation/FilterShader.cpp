#include "commonImport.h"
#include "FilterShader.h"
#include "Simulation.h"
#include "SampleAllocator.h"

#include <string>

class Simulation;

void FilterShader::onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count) {
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

void FilterShader::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) {
   std::cout << "onContact" << std::endl;
}

bool FilterShader::pairContient(const physx::PxContactModifyPair& pair, const physx::PxU32& flag) {
   return (getFlag(pair, 0) == flag) || (getFlag(pair, 1) == flag);
}

physx::PxU32 FilterShader::getFlag(const physx::PxContactModifyPair& pair, const short& index) {
   return getFlag(getFilterData(pair, index));
}

physx::PxU32 FilterShader::getFlag(const physx::PxFilterData& filterData) {
   return filterData.word0;
}

physx::PxFilterData FilterShader::getFilterData(const physx::PxContactModifyPair& pair, const short& index) {
   return pair.shape[index]->getSimulationFilterData();
}

physx::PxFilterData FilterShader::getOtherFilterData(const physx::PxContactModifyPair& pair, const physx::PxU32& flag) {
   physx::PxFilterData otherFilterData = physx::PxFilterData();
   if (pairContient(pair, flag)) {
      otherFilterData = (getFlag(pair, 0) == flag) ? getFilterData(pair, 1) : getFilterData(pair, 0);
   }
   return otherFilterData;
}

void FilterShader::ignoreContact(physx::PxContactModifyPair& pair) {
   for (int i = 0; i < pair.contacts.size(); i++) {
      pair.contacts.ignore(i);
   }
}

void FilterShader::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) {
   std::cout << "onConstraintBreak" << std::endl;
}
void FilterShader::onWake(physx::PxActor** actors, physx::PxU32 count) {
   std::cout << "onWake" << std::endl;
}
void FilterShader::onSleep(physx::PxActor** actors, physx::PxU32 count) {
   std::cout << "onSleep" << std::endl;
}
void FilterShader::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) {
   std::cout << "onTrigger" << std::endl;
}
void FilterShader::onAdvance(const physx::PxRigidBody*const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) {
   std::cout << "onAdvance" << std::endl;
}

void FilterShader::setupFiltering(physx::PxRigidActor* actor, physx::PxU32 filterGroup, physx::PxU32 filterMask)
{
   physx::PxFilterData filterData;
   filterData.word0 = filterGroup; // word0 = own ID
   filterData.word1 = filterMask;  // word1 = ID mask to filter pairs that trigger a
                                   // contact callback;
   const physx::PxU32 numShapes = actor->getNbShapes();
   physx::PxShape** shapes = (physx::PxShape**)SAMPLE_ALLOC(sizeof(physx::PxShape*)*numShapes);
   actor->getShapes(shapes, numShapes);
   for (physx::PxU32 i = 0; i < numShapes; i++)
   {
      physx::PxShape* shape = shapes[i];
      shape->setSimulationFilterData(filterData);
   }
   SAMPLE_FREE(shapes);
}

physx::PxFilterFlags FilterShader::filterCallback(
   physx::PxFilterObjectAttributes attributes0,
   physx::PxFilterData filterData0,
   physx::PxFilterObjectAttributes attributes1,
   physx::PxFilterData filterData1,
   physx::PxPairFlags& pairFlags,
   const void* constantBlock,
   physx::PxU32 constantBlockSize)
{
   // let triggers through
   if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
   {
      pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
      return physx::PxFilterFlag::eDEFAULT;
   }
   // generate contacts for all that were not filtered above
   pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT;
   // trigger the contact callback for pairs (A,B) where
   // the filtermask of A contains the ID of B and vice versa.
   if (filterData0.word0 == Simulation::FilterGroup::eBALLE && filterData1.word0 != Simulation::FilterGroup::eCARGO && filterData1.word0 != Simulation::FilterGroup::eSOL)
      return physx::PxFilterFlag::eSUPPRESS;

   if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
      pairFlags |= physx::PxPairFlag::eMODIFY_CONTACTS;
   return physx::PxFilterFlag::eDEFAULT;
}