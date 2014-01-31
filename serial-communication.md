Serial Communication Documentation
==================================

This is a list of commands that the Arduino will accept over serial. The commands should be sent as strings with a newline character at the end.

Status Reporting
----------------

The Arduino sends the current angle for every joint in every loop iteration with the following syntax:

armStatus [joint0angle] [joint1angle] [joint2angle] [joint3angle] [joint4angle] [joint0error] [joint1error] [joint2error] [joint3error] [joint4error]

* joint[0-4]angle
  Float between 0.0 and 360.0
* joint[0-4]error
  Float between 0.0 and 360.0

Commands
--------

* setJointAngle [jointNumber] [angle]
* getJointAngle [jointNumber]
* getJointLimits [jointNumber]
* setJointGains [jointNumber] [PGain] [IGain] [DGain]
* getJointGains [jointNumber]
* getjointError [jointNumber]


### setJointAngle [jointNumber] [angle]

#### Description

The Arduino sets the selected joint to the provided angle.

#### Params

* jointNumber
  Integer between 0 and 4
* angle
  Integer between 0 and 359

#### Response

jointSetPoint [jointNumber] [angle]

* jointNumber
  Integer between 0 and 4
* angle
  Integer between 0 and 359


### getJointAngle [jointNumber]

#### Description

The Arduino gets the current encoder angle for the provided joint and sends it back in the response.

#### Params

* jointNumber
  Integer between 0 and 4

#### Response

jointAngle [jointNumber] [angle]

* jointNumber
  Integer between 0 and 4
* angle
  Integer between 0 and 359


### getJointLimits [jointNumber]

#### Description

The Arduino gets the current encoder angle for the provided joint and sends it back in the response.

#### Params

* jointNumber
  Integer between 0 and 4

#### Response

jointLimits [jointNumber] [min] [max]

* jointNumber
  Integer between 0 and 4
* min
  Integer between 0 and 359
* max
  Integer between 0 and 359

### getJointGains [jointNumber]

#### Description

The Arduino will return the PID gains for the current joint.

#### Params

* jointNumber
  Integer between 0 and 4

#### Response

jointGains [jointNumber] [PGain] [IGain] [DGain]

* jointNumber
  Integer between 0 and 4
* PGain
  Float between 0.0 and 5.0
* IGain
  Float between 0.0 and 5.0
* DGain
  Float between 0.0 and 5.0

### setJointGains [jointNumber] [PGain] [IGain] [DGain]

#### Description

The Arduino will set the provided values as the joint PID gains.

#### Params

* jointNumber
  Integer between 0 and 4
* PGain
  Float between 0.0 and 5.0 and must always be 2 digits after the decimal place
* IGain
  Float between 0.0 and 5.0 and must always be 2 digits after the decimal place
* DGain
  Float between 0.0 and 5.0 and must always be 2 digits after the decimal place

#### Response

No response

### getJointError [jointNumber]

#### Description

The Arduino will return the current angle error for the provided joint.

#### Params

* jointNumber
  Integer between 0 and 4

#### Response

jointError [jointNumber] [error]

* jointNumber
  Integer between 0 and 4
* error
  Float between -180.0 and 180.0