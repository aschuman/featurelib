#ifndef EUCLIDEANDISTANCE_H
#define EUCLIDEANDISTANCE_H

#include "Distance.h"

#include <cassert>




// fix the std::vector here
template <typename V_T>
class EuclideanDistance : public Distance<V_T>
{
public:
    typedef V_T   VecType;

public:
    EuclideanDistance();
    virtual ~EuclideanDistance();

    virtual double compute(const VecType& f1, const VecType& f2) const;
};





template <typename V_T>
EuclideanDistance<V_T>::EuclideanDistance()
{}




template <typename V_T>
EuclideanDistance<V_T>::~EuclideanDistance()
{}



template <typename V_T>
double EuclideanDistance<V_T>::compute(const V_T& f1,
                                       const V_T& f2) const
{
    assert(f1.size() == f2.size());

    double dist = 0.0;
    for (size_t ii = 0; ii < f1.size(); ++ii)
    {
        dist += (f1[ii] - f2[ii]) * (f1[ii] - f2[ii]);
    }
    return sqrt(dist);
}

#endif
