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

#ifndef RENDER_SNIPPET

#define RENDER_SNIPPET

#include "commonImport.h"
#include "SnippetCamera.h"

extern void initPhysics(bool interactive);
extern void stepPhysics(bool interactive);
extern void cleanupPhysics(bool interactive);
extern void keyPress(unsigned char key, physx::PxTransform& camera);

class SimulationRender {

private:
   //static void initPhysics(bool interactive);
   //static void stepPhysics(bool interactive);
   //static void cleanupPhysics(bool interactive);
   //static void keyPress(unsigned char key, const physx::PxTransform& camera);

   Camera* sCamera;

   // SINGLETON
protected:
   SimulationRender(){}
   static SimulationRender& get() {
      static SimulationRender singleton; // definition
      return singleton;
   }
   
public:
   static Camera* getCamera() {
      return get().sCamera;
   }

   static void setCamera(Camera* camera) {
      get().sCamera = camera;
   }

   static void motionCallback(int x, int y);

   static void keyboardCallback(unsigned char key, int x, int y);

   static void mouseCallback(int button, int state, int x, int y);

   static void mouseWheelCallback(int button, int state, int x, int y);

   static void idleCallback();

   static void renderCallback();

   static void exitCallback(void);

   static void renderLoop();
};
#endif // !RENDER_SNIPPET
