// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2018 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

// ****************************************************************************
// This snippet illustrates simple use of physx
//
// It creates a number of box stacks on a plane, and if rendering, allows the
// user to create new stacks and fire a ball from the camera position
// ****************************************************************************

#ifndef SIMULATION_H

#define SIMULATION_H

#include "commonImport.h"
#include "Incopiable.h"
#include "SimulationRender.h"
#include "MyCallback.h"
#include "Minuteur.h"

#include <chrono>
#include <vector>
#include <map>

class Simulation : public Incopiable, SimulationRender {

   physx::PxDefaultAllocator		gAllocator;
   physx::PxDefaultErrorCallback	gErrorCallback;

   physx::PxFoundation*			   gFoundation;
   physx::PxPhysics*				   gPhysics;

   physx::PxDefaultCpuDispatcher*	gDispatcher;
   physx::PxScene*				      gScene;

   physx::PxMaterial*				   gMaterial;

   physx::PxPvd*                  gPvd;

   physx::PxReal stackZ;

   physx::PxTransform positionWall;

   unsigned short dTBalls;
   unsigned short nbBallsMax;
   unsigned short nbBalls;
   signed short vitesseBall;

   unsigned short dTArret;
   //unsigned short dtChronoOld;
   Minuteur minuteur;

   //std::chrono::time_point<std::chrono::high_resolution_clock> oldTime;

   physx::PxRigidDynamic* pWall;
   std::vector<physx::PxRigidDynamic*> listBall;
   std::vector<unsigned short> listIdBallTouched;
   bool lastBallTouch = false;

   MyCallback filterShader;

   struct FilterGroup
   {
      enum TypeObject
      {
         eWALL = (1 << 0),
      };
   };

   // SINGLETON
   Simulation() :
      gAllocator{},
      gErrorCallback{},
      gFoundation{ NULL },
      gPhysics{ NULL },
      gDispatcher{ NULL },
      gScene{ NULL },
      gMaterial{ NULL },
      gPvd{ NULL },
      stackZ{ 10.0f },
      positionWall{ physx::PxTransform(physx::PxVec3(15.0f, 8.0f, 0.0f)) },
      dTBalls{ 3 }, //3 secondes entre chaque balle
      nbBallsMax{ 4 },
      nbBalls{ 0 },
      vitesseBall{ 50 },
      dTArret{ 10 },
      //dtChronoOld{ 0 },
      //oldTime{ std::chrono::high_resolution_clock::now() }
      minuteur{}
   {}

public:
   static Simulation& get() {
      static Simulation singleton; // definition
      return singleton;
   }

   physx::PxRigidDynamic* createBall(const physx::PxTransform& t,
      const physx::PxReal& rayon,
      const physx::PxVec3& velocity = physx::PxVec3(0),
      const unsigned short indexBall = 0);
public:
   static physx::PxU32 getBallFilterGroup(const unsigned short indexBall) {
      return (1 << indexBall);
   }
   static physx::PxU32 getWallFilterGroup() {
      return FilterGroup::eWALL;
   }
public:
   physx::PxRigidDynamic* createWall(const physx::PxTransform& t, physx::PxReal halfExtendX, physx::PxReal halfExtendY, physx::PxReal halfExtendZ);

   void initPhysics(bool interactive);

   void stepPhysics(bool interactive);

   void cleanupPhysics(bool interactive);

   void keyPress(unsigned char key, const physx::PxTransform& camera);

   // DESTRUCTEUR

   ~Simulation() {
      cleanupPhysics(true);
   }

   // FILTERING

   void setupFiltering(physx::PxRigidActor* actor, physx::PxU32 filterGroup, physx::PxU32 filterMask);

   static physx::PxFilterFlags myFilterShader(
      physx::PxFilterObjectAttributes attributes0,
      physx::PxFilterData filterData0,
      physx::PxFilterObjectAttributes attributes1,
      physx::PxFilterData filterData1,
      physx::PxPairFlags& pairFlags,
      const void* constantBlock,
      physx::PxU32 constantBlockSize);

   //MAIN
public:
   int snippetMain(int, const char*const*);

   physx::PxRigidDynamic* getWall() {
      return pWall;
   }

   void getWall(physx::PxRigidDynamic* wall) {
      pWall = wall;
   }

   short getNbBallMax() {
      return nbBallsMax;
   }

   void contactDetected(const unsigned short indexBall) {
      listIdBallTouched.push_back(indexBall);
   }

private:
   void gestionCollision(const unsigned short indexBall);
};
#endif