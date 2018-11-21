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

#include <ctype.h>

#include "../SnippetCommon/SnippetPrint.h"
#include "../SnippetCommon/SnippetPVD.h"
#include "../SnippetUtils/SnippetUtils.h"

#include "SampleAllocator.h"
#include "PxFiltering.h"
#include "Simulation.h"

#include <sstream>
#include <chrono>

physx::PxRigidDynamic* Simulation::createVehicule(const physx::PxTransform& t, physx::PxReal halfExtendX, physx::PxReal halfExtendY, physx::PxReal halfExtendZ) {

   std::unique_ptr<physx::PxShape, std::function<void(physx::PxBase*)>> shape = std::unique_ptr<physx::PxShape, std::function<void(physx::PxBase*)>>(
      gPhysics->createShape(physx::PxBoxGeometry(halfExtendX, halfExtendY, halfExtendZ), *gMaterial, true, physx::PxShapeFlag::eSIMULATION_SHAPE)
      , Simulation::elementPhysXBaseDeleter
      );
   
   pVehicule = std::make_unique<Vehicule>(10.f, t, shape.get(), gPhysics.get(), gMaterial.get());

   gScene->addActor(*pVehicule->getRigidBody());

   physx::PxU32 filterMask = 0;
   filterMask |= FilterGroup::eBALLE;
   filterMask |= FilterGroup::eCARGO;
   setupFiltering(pVehicule->getRigidBody(), FilterGroup::eVEHICULE, filterMask);

   return pVehicule->getRigidBody();
}
physx::PxRigidDynamic* Simulation::createCargo(const physx::PxTransform& t, physx::PxReal halfExtendX, physx::PxReal halfExtendY, physx::PxReal halfExtendZ) {
   physx::PxShape* shape = gPhysics->createShape(physx::PxBoxGeometry(halfExtendX, halfExtendY, halfExtendZ), *gMaterial, true, physx::PxShapeFlag::eSIMULATION_SHAPE);
   pCargo = std::unique_ptr<physx::PxRigidDynamic, std::function<void(physx::PxBase*)>>(gPhysics->createRigidDynamic(t), Simulation::elementPhysXBaseDeleter);
   pCargo->attachShape(*shape);
   physx::PxRigidBodyExt::updateMassAndInertia(*pCargo, 10.0f);
   pCargo->setRigidDynamicLockFlags(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X | physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z);
   gScene->addActor(*pCargo);
   pCargo->setSleepThreshold(physx::PxReal{ 0 });
   shape->release();

   physx::PxU32 filterMask = 0;
   filterMask |= FilterGroup::eBALLE;
   filterMask |= FilterGroup::eVEHICULE;
   filterMask |= FilterGroup::eGOAL;
   setupFiltering(pCargo.get(), FilterGroup::eCARGO, filterMask);

   return pCargo.get();
}
physx::PxRigidDynamic* Simulation::createGoal(const physx::PxTransform& t, physx::PxReal rayon, physx::PxReal hauteur) {

   pGoal = std::make_unique<Goal>(rayon, hauteur, t, gPhysics.get(), gMaterial.get());

   gScene->addActor(*pGoal->getRigidBody());

   physx::PxU32 filterMask = 0;
   filterMask |= FilterGroup::eCARGO;
   setupFiltering(pGoal->getRigidBody(), FilterGroup::eGOAL, filterMask);

   return pGoal->getRigidBody();
}

void Simulation::initPhysics(bool interactive)
{
   initSampleAllocator();

   gFoundation = std::unique_ptr<physx::PxFoundation, std::function<void(physx::PxFoundation*)>>(PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback), Simulation::elementPhysXFoundationDeleter);

   gPvd = std::unique_ptr<physx::PxPvd, std::function<void(physx::PxPvd*)>>(PxCreatePvd(*gFoundation), Simulation::elementPhysXPvdDeleter);
   physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
   gPvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

   gPhysics = std::unique_ptr<physx::PxPhysics, std::function<void(physx::PxPhysics*)>>(PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, physx::PxTolerancesScale(), true, gPvd.get()), Simulation::elementPhysXPhysicsDeleter);

   physx::PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
   // On met la gravite
   sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
   gDispatcher = std::unique_ptr<physx::PxDefaultCpuDispatcher, std::function<void(physx::PxDefaultCpuDispatcher*)>>(physx::PxDefaultCpuDispatcherCreate(2), Simulation::elementPhysXDispatcherDeleter);
   sceneDesc.cpuDispatcher = gDispatcher.get();
   sceneDesc.filterShader = myFilterShader;
   gScene = std::unique_ptr<physx::PxScene, std::function<void(physx::PxScene*)>>(gPhysics->createScene(sceneDesc), Simulation::elementPhysXSceneDeleter);

   gScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);

   physx::PxPvdSceneClient* pvdClient = gScene->getScenePvdClient();
   if (pvdClient)
   {
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
      pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
   }
   gMaterial = std::unique_ptr<physx::PxMaterial, std::function<void(physx::PxBase*)>>(gPhysics->createMaterial(0.5f, 0.5f, 0.6f), Simulation::elementPhysXBaseDeleter);

   physx::PxRigidStatic* groundPlane = PxCreatePlane(*gPhysics, physx::PxPlane(0, 1, 0, 0), *gMaterial);
   groundPlane->setActorFlags(physx::PxActorFlag::eVISUALIZATION);
   gScene->addActor(*groundPlane);

   createVehicule(positionVehiculeInit, 5.0f, 5.0f, 5.0f);
   createGoal(positionGoalInit, 100.f,100.f);

   gScene->setContactModifyCallback(&filterShader);
   gScene->setSimulationEventCallback(&filterShader);

}

void Simulation::stepPhysics(bool interactive)
{
   PX_UNUSED(interactive);
   if (tirDemande) {
      if (!minuteur.isConfigured()) {
         minuteur.setDecompte(dTBalls);
         minuteur.start();
         auto balle = pVehicule->tir(rayonBall, vitesseBall);

         physx::PxU32 filterMask = 0;
         filterMask |= FilterGroup::eBALLE;
         filterMask |= FilterGroup::eVEHICULE;
         filterMask |= FilterGroup::eGOAL;
         filterMask |= FilterGroup::eCARGO;
         setupFiltering(balle->getRigidBody(), FilterGroup::eBALLE, filterMask);

      }
      else {
         minuteur.refresh();
         if (minuteur.isFinished()) {
         }
      }
      // on desamorce la demande de tir
      tirDemande = false;
   }
   //Quand la derniere balle a touchee, on eteint la simulation au bout de 5 secondes
   if (goalTouch) {
      changeGoalPosition(&positionGoalInit);
   }

   gScene->simulate(1.0f / 60.0f);
   gScene->fetchResults(true);
}

void Simulation::cleanupPhysics(bool interactive)
{
   if (!sceneCleanUp) {
      PX_UNUSED(interactive);
      gScene->release();
      gDispatcher->release();
      gPhysics->release();
      physx::PxPvdTransport* transport = gPvd->getTransport();
      gPvd->release();
      transport->release();

      gFoundation->release();

      sceneCleanUp = true;

      printf("PhysX Exercice 2 done.\n");
   }
}

void Simulation::keyPress(unsigned char key, const physx::PxTransform& camera)
{
   switch (toupper(key))
   {
   case ' ':	tirDemande = true;	break;
   }
}

int Simulation::snippetMain(int, const char*const*)
{
#ifdef RENDER_SNIPPET
   renderLoop();
#else
   static const physx::PxU32 frameCount = 100;
   initPhysics(false);
   for (physx::PxU32 i = 0; i < frameCount; i++)
      stepPhysics(false);
   cleanupPhysics(false);
#endif

   return 0;
}
// FILTERING

void Simulation::setupFiltering(physx::PxRigidActor* actor, physx::PxU32 filterGroup, physx::PxU32 filterMask)
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

physx::PxFilterFlags Simulation::myFilterShader(
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
   if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
      pairFlags |= physx::PxPairFlag::eMODIFY_CONTACTS;
   return physx::PxFilterFlag::eDEFAULT;
}




// EXTERN FONCTION

void initPhysics(bool interactive) {
   Simulation::get().initPhysics(interactive);
}
void stepPhysics(bool interactive) {
   Simulation::get().stepPhysics(interactive);
}
void cleanupPhysics(bool interactive) {
   Simulation::get().cleanupPhysics(interactive);
}
void keyPress(unsigned char key, physx::PxTransform& camera) {
   Simulation::get().keyPress(key, camera);
}

/*
// GESTION COLLISION
void Simulation::gestionCollision(const unsigned short indexBall) {
   physx::PxRigidDynamic* ball = listBall[indexBall - 1];
   switch (indexBall) {
   case 1: break;
   case 2: break;
   case 3: ball->setLinearVelocity(physx::PxVec3(0.f, 50.f, 0.f)); break;
   case 4:
      if (ball->getLinearVelocity().magnitude() != 0.5f*vitesseBall) {
         ball->setLinearVelocity(0.5f*ball->getLinearVelocity());
         lastBallTouch = true;
      }
      break;
   default: break;
   }

}
*/