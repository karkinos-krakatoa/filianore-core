#ifndef _FRESNEL_THIN_FILM_H
#define _FRESNEL_THIN_FILM_H

#include "fresnel.h"

namespace filianore {

class ThinFilmInteference : public Fresnel {
public:
    ThinFilmInteference(float _thinFilmIOR, float _thickness, float _mediumIOR)
        : thinFilmIOR(_thinFilmIOR), thickness(_thickness), mediumIOR(_mediumIOR) {
    }

    PrincipalSpectrum evaluate(float cosThetaI) const;

private:
    const float extIOR = 1.f;
    const float thinFilmIOR, thickness, mediumIOR;
};

} // namespace filianore

#endif