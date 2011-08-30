/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "CCAccelerometer_android.h"
#include "jni/SensorJni.h"
#include <stdio.h>
#include <android/log.h>

#define TG3_GRAVITY_EARTH                    (9.80665f)
#define  LOG_TAG    "CCAccelerometer_android"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

namespace cocos2d
{
	CCAccelerometer* CCAccelerometer::m_spCCAccelerometer = NULL;

	CCAccelerometer::CCAccelerometer() : m_pAccelDelegate(NULL)
	{
	}

    CCAccelerometer::~CCAccelerometer() 
	{
		m_spCCAccelerometer = NULL;
    }

    CCAccelerometer* CCAccelerometer::sharedAccelerometer() 
	{

    	if (m_spCCAccelerometer == NULL)
    	{
    		m_spCCAccelerometer = new CCAccelerometer();
    	}
    	
    	return m_spCCAccelerometer;
    }

    void CCAccelerometer::removeDelegate(CCAccelerometerDelegate* pDelegate) 
	{
		disableAccelerometerJNI();
    }

    void CCAccelerometer::addDelegate(CCAccelerometerDelegate* pDelegate) 
	{
		m_pAccelDelegate = pDelegate;
		enableAccelerometerJNI();
    }

    void CCAccelerometer::update(float x, float y, float z, long sensorTimeStamp) 
	{
		m_obAccelerationValue.x = -((double)x / TG3_GRAVITY_EARTH);
		m_obAccelerationValue.y = -((double)y / TG3_GRAVITY_EARTH);
		m_obAccelerationValue.z = -((double)z / TG3_GRAVITY_EARTH);
		m_obAccelerationValue.timestamp = (double)sensorTimeStamp;

		m_pAccelDelegate->didAccelerate(&m_obAccelerationValue);
    }
} // end of namespace cococs2d

