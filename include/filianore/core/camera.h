#ifndef _CAMERA_H
#define _CAMERA_H


#include "../maths/static_array.h"
#include "../maths/transform.h"


namespace filianore
{

	struct CameraSample 
	{
    	StaticArray<float, 2> pFilm;
    	StaticArray<float, 2> pLens;
    	float time;
	};


	class Camera
	{
	public:
		Camera(const Transform& _cameraTransform, float _shutterOpen, float _shutterClose, Film* film);

		virtual ~Camera() { }


		virtual float AwakenRay(const CameraSample& _cameraSample, Ray* ray) const = 0;


		Transform cameraTransform;
		const float shutterOpen, shutterClose;
		Film* film;
	};
    
}


#endif