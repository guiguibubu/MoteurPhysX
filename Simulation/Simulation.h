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
#include "FilterShader.h"
#include "Minuteur.h"
#include "Balle.h"
#include "Vehicule.h"
#include "Goal.h"
#include "Bot.h"

#include <chrono>
#include <vector>
#include <deque>
#include <map>

class Balle;
class Vehicule;
class Goal;

class Simulation : public Incopiable, SimulationRender {
public:
   static const physx::PxReal FREQUENCE_SIMULATION;
private:
   physx::PxDefaultAllocator		gAllocator;
   physx::PxDefaultErrorCallback	gErrorCallback;

   std::unique_ptr<physx::PxFoundation, std::function<void(physx::PxFoundation*)>>			             gFoundation;
   std::unique_ptr<physx::PxPhysics, std::function<void(physx::PxPhysics*)>>				             gPhysics;

   std::unique_ptr<physx::PxDefaultCpuDispatcher, std::function<void(physx::PxDefaultCpuDispatcher*)>>	 gDispatcher;
   std::unique_ptr<physx::PxScene, std::function<void(physx::PxScene*)>>				                 gScene;

   std::unique_ptr<physx::PxMaterial, std::function<void(physx::PxBase*)>>				                 gMaterial;

   std::unique_ptr<physx::PxPvd, std::function<void(physx::PxPvd*)>>                                     gPvd;

   physx::PxReal stackZ;

   physx::PxTransform positionVehiculeInit;
   physx::PxTransform positionCargoInit;
   physx::PxTransform positionGoalInit;

   bool sceneCleanUp = false;

   Minuteur minuteur;

   std::deque<std::unique_ptr<Balle>> listBall;
   std::vector<unsigned short> listIdBallTouched;

   bool goalTouch = false;
   bool tirDemande = false;

   std::unique_ptr<physx::PxRigidStatic, std::function<void(physx::PxBase*)>> pSol;
   std::unique_ptr<Vehicule> pVehicule;
   std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>> pCargo;
   std::unique_ptr<Goal> pGoal;
   std::unique_ptr<Bot> pBot;

   FilterShader filterShader;

   // SINGLETON
   Simulation() :
      gAllocator{},
      gErrorCallback{},
      stackZ{ 10.0f },
      positionVehiculeInit{ physx::PxTransform(physx::PxVec3(0.f, 5.f, 0.f)) },
      positionCargoInit{ physx::PxTransform(physx::PxVec3(20.f, 5.f, 0.f)) },
      positionGoalInit{ physx::PxTransform(physx::PxVec3(100.f, 50.f, 0.f)) },
      minuteur{ true },
      filterShader{ FilterShader::FilterShader() }
   {
   }

public:
   static Simulation& get() {
      static Simulation singleton; // definition
      return singleton;
   }

   physx::PxRigidStatic* createSol();
   physx::PxRigidDynamic* createVehicule(const physx::PxTransform& t, physx::PxReal halfExtendX, physx::PxReal halfExtendY, physx::PxReal halfExtendZ);
   physx::PxRigidDynamic* createCargo(const physx::PxTransform& t, physx::PxReal halfExtendX, physx::PxReal halfExtendY, physx::PxReal halfExtendZ);
   physx::PxRigidDynamic* createGoal(const physx::PxTransform& t, physx::PxReal rayon, physx::PxReal hauteur);
   physx::PxRigidDynamic* tirBalle(const Vehicule& vehicule, const physx::PxVec3& directionTir = {1,0,0});

public:

   enum FilterGroup
   {
      eSOL = (1 << 0),
      eVEHICULE = (1 << 1),
      eCARGO = (1 << 2),
      eGOAL = (1 << 3),
      eBALLE = (1 << 4)
   };

public:

   void initPhysics(bool interactive);

   void stepPhysics(bool interactive);

   void cleanupPhysics(bool interactive);

   void keyPress(unsigned char key, const physx::PxTransform& camera);

   // DESTRUCTEUR

   ~Simulation() {
      cleanupPhysics(true);
   }

   static void elementPhysXFoundationDeleter(physx::PxFoundation* elementPhysX) {
      elementPhysX->release();
   }
   static void elementPhysXPhysicsDeleter(physx::PxPhysics* elementPhysX) {
      elementPhysX->release();
   }
   static void elementPhysXDispatcherDeleter(physx::PxDefaultCpuDispatcher* elementPhysX) {
      elementPhysX->release();
   }
   static void elementPhysXSceneDeleter(physx::PxScene* elementPhysX) {
      elementPhysX->release();
   }
   static void elementPhysXBaseDeleter(physx::PxBase* elementPhysX) {
      elementPhysX->release();
   }
   static void elementPhysXPvdDeleter(physx::PxPvd* elementPhysX) {
      elementPhysX->release();
   }

   //MAIN
public:
   int snippetMain(int, const char*const*);

   Vehicule* getVehicule() {
      return pVehicule.get();
   }

   void goalAtteint() {
      goalTouch = true;
      pGoal->changePosition();
   }
};
#endif