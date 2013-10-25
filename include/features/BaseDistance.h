#ifndef BASEDISTANCE_H
#define BASEDISTANCE_H





template <typename OutputType_T>
class BaseDistance
{
public:
    typedef OutputType_T    OutputType;

public:
    BaseDistance();
    virtual ~BaseDistance();

    virtual double compute(const OutputType& f1,
                           const OutputType& f2) const = 0;
};





template <typename OT_T>
BaseDistance<OT_T>::BaseDistance()
{}



template <typename OT_T>
BaseDistance<OT_T>::~BaseDistance()
{}

#endif
