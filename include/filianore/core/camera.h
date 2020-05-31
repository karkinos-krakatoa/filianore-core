#pragma once


#include "../math/transform.h"


namespace filianore
{

	template <typename T> struct CameraSample 
	{
    	StaticArray<T, 2> pFilm;
    	StaticArray<T, 2> pLens;
    	T time;
	};


	template <typename T, size_t N> class Camera
	{
	public:
		Camera(const Transform<T>& _cameraTransform, float _shutterOpen, float _shutterClose, Film<T> *film);

		virtual ~Camera() { }


		virtual T AwakenRay(const CameraSample<T>& _cameraSample, Ray<T, N>* ray) const = 0;


		Transform<T> cameraTransform;
		const T shutterOpen, shutterClose;
		Film<T>* film;

	};
    
}