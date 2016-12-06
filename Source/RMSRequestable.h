/*
  ==============================================================================

    RMSRequestable.h
    Created: 6 Dec 2016 10:23:49am
    Author:  Jonathan

  ==============================================================================
*/

#ifndef RMSREQUESTABLE_H_INCLUDED
#define RMSREQUESTABLE_H_INCLUDED

class RMSRequestable
{
public:
  RMSRequestable() : currentRMS(0) {};
  virtual ~RMSRequestable() {};

  float getCurrentRMS() const { return currentRMS; };
  void setCurrentRMS(float rms) { currentRMS = rms; }
private:
  float currentRMS;
};



#endif  // RMSREQUESTABLE_H_INCLUDED
