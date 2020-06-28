#pragma once


#include "math/static_array.h"
#include "math/transform.h"


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
		Camera(const Transform<float>& _cameraTransform, float _shutterOpen, float _shutterClose, Film* film);

		virtual ~Camera() { }


		virtual float AwakenRay(const CameraSample& _cameraSample, Ray* ray) const = 0;


		Transform<float> cameraTransform;
		const float shutterOpen, shutterClose;
		Film* film;
	};
    
}