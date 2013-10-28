#ifndef DISTANCE_H
#define DISTANCE_H





template <typename O_T>
class Distance
{
public:
    typedef O_T    OutputType;

public:
    Distance();
    virtual ~Distance();

    virtual double compute(const OutputType& f1,
                           const OutputType& f2) const = 0;
};





template <typename O_T>
Distance<O_T>::Distance()
{}



template <typename O_T>
Distance<O_T>::~Distance()
{}

#endif
