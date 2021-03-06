#ifndef EUCLIDEANDISTANCE_H
#define EUCLIDEANDISTANCE_H

#include "Distance.h"

#include <cassert>
#include <vector>
#include <cmath>





//TODO: also here add static asserts to limit S_T to number types
template <typename S_T>
class EuclideanDistance : public Distance<S_T >
{
public:
    typedef S_T   ScalarType;

public:
    EuclideanDistance();
    virtual ~EuclideanDistance();

    virtual double compute(const ScalarType& f1,
                           const ScalarType& f2) const;
};





template <typename S_T>
EuclideanDistance<S_T>::EuclideanDistance()
{}




template <typename S_T>
EuclideanDistance<S_T>::~EuclideanDistance()
{}



template <typename S_T>
double EuclideanDistance<S_T>::compute(const S_T& f1,
                                       const S_T& f2) const
{
    assert(f1.size() == f2.size());

    double dist = 0.0;
    for (std::size_t ii = 0; ii < f1.size(); ++ii)
    {
        dist += (f1[ii] - f2[ii]) * (f1[ii] - f2[ii]);
    }
    return sqrt(dist);
}

#endif
