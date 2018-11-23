#ifndef FILTER_SHADER_H

#define FILTER_SHADER_H

#include "PxSimulationEventCallback.h"
#include "PxContactModifyCallback.h"
#include <vector>

class FilterShader : public physx::PxContactModifyCallback, public physx::PxSimulationEventCallback {

public:
   FilterShader() = default;
   ~FilterShader() = default;

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

public:
   static void setupFiltering(physx::PxRigidActor* actor, physx::PxU32 filterGroup, physx::PxU32 filterMask);

   static physx::PxFilterFlags filterCallback(
      physx::PxFilterObjectAttributes attributes0,
      physx::PxFilterData filterData0,
      physx::PxFilterObjectAttributes attributes1,
      physx::PxFilterData filterData1,
      physx::PxPairFlags& pairFlags,
      const void* constantBlock,
      physx::PxU32 constantBlockSize);
};

#endif // !FILTER_SHADER_H


