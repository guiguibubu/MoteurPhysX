#ifndef MY_CALLBACK_H

#define MY_CALLBACK_H

#include "PxSimulationEventCallback.h"
#include "PxContactModifyCallback.h"
#include <vector>

class MyCallback : public physx::PxContactModifyCallback, public physx::PxSimulationEventCallback {

public:
   MyCallback() = default;
   ~MyCallback() = default;

   //from PxContactModifyCallback
   void onContactModify(physx::PxContactModifyPair* const pairs, physx::PxU32 count);
   //from PxSimulationEventCallback
   void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
   void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count);
   void onWake(physx::PxActor** actors, physx::PxU32 count);
   void onSleep(physx::PxActor** actors, physx::PxU32 count);
   void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
   void onAdvance(const physx::PxRigidBody*const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count);

private:
   static bool pairContient(const physx::PxContactModifyPair& pairs, const physx::PxU32& flag);
   static physx::PxU32 getFlag(const physx::PxContactModifyPair& pair, const short& index);
   static physx::PxFilterData getFilterData(const physx::PxContactModifyPair& pair, const short& index);
   static physx::PxU32 getFlag(const physx::PxFilterData& filterData);
   static physx::PxFilterData getOtherFilterData(const physx::PxContactModifyPair& pair, const physx::PxU32& flag);
   static void ignoreContact(physx::PxContactModifyPair& pair);
};

#endif // !FILTER_SHADER


