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

#include <vector>
#include "commonImport.h"

#include "SnippetRender.h"

#include "SimulationRender.h"
#include <chrono>

      extern void initPhysics(bool interactive);
      extern void stepPhysics(bool interactive);
      extern void cleanupPhysics(bool interactive);
      extern void keyPress(unsigned char key, physx::PxTransform& camera);


      void SimulationRender::motionCallback(int x, int y)
      {
         SimulationRender::getCamera()->handleMotion(x, y);
         std::cout << "xDirCam =" << SimulationRender::getCamera()->getDir().x << std::endl;
         std::cout << "yDirCam =" << SimulationRender::getCamera()->getDir().y << std::endl;
         std::cout << "zDirCam =" << SimulationRender::getCamera()->getDir().z << std::endl;
      }

      void SimulationRender::keyboardCallback(unsigned char key, int x, int y)
      {
         if (key == 27)
            exit(0);

         if (!SimulationRender::getCamera()->handleKey(key, x, y, 2.f))
            keyPress(key, SimulationRender::getCamera()->getTransform());
      }

      void SimulationRender::mouseCallback(int button, int state, int x, int y)
      {
         SimulationRender::getCamera()->handleMouse(button, state, x, y);
      }

      void SimulationRender::idleCallback()
      {
         glutPostRedisplay();
      }

      void SimulationRender::renderCallback()
      {
         stepPhysics(true);
         Render::startRender(SimulationRender::getCamera()->getEye(), SimulationRender::getCamera()->getDir());

         physx::PxScene* scene;
         PxGetPhysics().getScenes(&scene, 1);
         physx::PxU32 nbActors = scene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
         if (nbActors)
         {
            std::vector<physx::PxRigidActor*> actors(nbActors);
            scene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<physx::PxActor**>(&actors[0]), nbActors);
            Render::renderActors(&actors[0], static_cast<physx::PxU32>(actors.size()), true);
         }

         Render::finishRender();
      }

      void SimulationRender::exitCallback(void)
      {
         delete SimulationRender::getCamera();
         cleanupPhysics(true);
      }

      void SimulationRender::renderLoop()
      {
         SimulationRender::setCamera(new Camera(physx::PxVec3(500.0f, 500.0f, 500.0f), physx::PxVec3(-0.5f, -0.5f, -0.5f)));

         Render::setupDefaultWindow("PhysX Exercice 2");
         Render::setupDefaultRenderState();

         glutIdleFunc(idleCallback);
         glutDisplayFunc(renderCallback);
         glutKeyboardFunc(keyboardCallback);
         glutMouseFunc(mouseCallback);
         glutMotionFunc(motionCallback);
         motionCallback(0, 0);

         atexit(exitCallback);

         initPhysics(true);

         glutMainLoop();

      }
