#ifndef FRAME_H
#define FRAME_H

#include <Arduino.h>
/**
 * class representing a frame as defined in the ALUP v.0.2
 */
class Frame
{
    public:
      //array containing the data of this frame
      byte* body;
      //the size of the data
      int32_t body_size;
      //the offset of the first body value
      int32_t offset;
      // the time at which the frame will be applied
      uint32_t timestamp;
      //the command byte
      uint8_t command;
      // frame id to associate frame with its acknowledgement
      uint8_t id;

      // incoming and outgoing timestamps of this frame
      // in ms, local system time
      uint32_t t_in;
      uint32_t t_out;

    ~Frame()
    {
      // delete the frame's body which was allocated
      // when reading it in
      free(this->body);
    }
    
};
enum Command
{
  NONE = 0,
  CLEAR = 1,
  DISCONNECT = 2,
  TOGGLE_INTERNAL_LED = 4
};

#endif