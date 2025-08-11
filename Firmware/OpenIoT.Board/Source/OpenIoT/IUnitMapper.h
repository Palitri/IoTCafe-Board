#ifndef IUnitMapper_h
#define IUnitMapper_h

/// <summary>
/// Maps a value to another value, simply acting like a function with a scalar input producing a scalar output.
/// </summary>
class IUnitMapper
{
public:
  virtual float Map(float value) = 0;
};

#endif
// IUnitMapper_h
